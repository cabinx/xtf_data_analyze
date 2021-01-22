#include "DataAccess/xtfDataAccess.h"

std::mutex xtfDataAccess::frameQueueMutex;

xtfDataAccess::xtfDataAccess(){
    //channelNum = 2;
    //curPingNum = 0;
    transSwitch = true;
    tState = transInit;
    frameQueue = std::queue<Frame*>();
}

xtfDataAccess::~xtfDataAccess(){}

int xtfDataAccess::xtfFilePath(string filePath){
    m_filePath = filePath;
    return 0;
}

int xtfDataAccess::xtfPingTrans(){
    m_file.open(m_filePath, std::ios::in|std::ios::binary);
    if(!m_file.is_open()){
        std::cout<<"Failed open xtf file : "<<m_filePath<<std::endl;
        return 1;
    }
    
    xtf_packetheader packetHeader;
    while(transSwitch){
        //read the file header
        if(tState == transInit){
            if(m_file.get()==0x7B){    //check the fileformat
                fileHeader.FileFormat = 0x7B;
                m_file.read((char *)&fileHeader+1, sizeof(fileHeader)-1);
                tState = transHaveFileHeader;
            }
            else{
                std::cout<<"xtf file header error..."<<std::endl;
                if(m_file.is_open()){
                    m_file.close();
                }
                return 2;
            }
        }

        //read the ping header
        if(tState == transHaveFileHeader){
            memset(&packetHeader,0,sizeof(xtf_packetheader));
            m_file.read((char*)&packetHeader, sizeof(char)*2);
            if(packetHeader.MagicNumber == XTF_MAGIC_NUMBER){    //check the MagicNumber
                m_file.read((char*)&packetHeader+2, sizeof(xtf_packetheader)-2);
                if(packetHeader.HeaderType == XTF_DATA_SIDESCAN){
                    tState = transHavePing;
                }
                else if(packetHeader.HeaderType == XTF_DATA_BATHYMETRY){
                    tState = transHaveBathymetry;
                }
                else if(packetHeader.HeaderType == XTF_DATA_ATTITUDE){
                    tState = transHaveAttitude;
                }
                else{
                    std::cout<<"packet header type error..."<<std::endl;
                    if(m_file.is_open()){
                        m_file.close();
                    }
                    return 3;
                }
            }
        }
        
        //read the chanheader and sidescan data of channel 0,1
        if(tState == transHavePing){
            xtf_pingpacket *pingPacket= new xtf_pingpacket;
            memset(pingPacket,0,sizeof(xtf_pingpacket));
            
            //load the pingheader
            pingPacket->pingHeader.packetHeader = packetHeader;
            m_file.read((char *)&pingPacket->pingHeader + sizeof(xtf_packetheader), sizeof(xtf_pingheader) - sizeof(xtf_packetheader));

            //read the chanheader of channel 0
            m_file.read((char *)&pingPacket->pingChanHeader[0], sizeof(xtf_pingchanheader));
            //read the data of channel 0
            unsigned short bytesPerSamplePort = fileHeader.chaninfo[pingPacket->pingChanHeader[0].ChannelNumber].BytesPerSample;
            unsigned int numSamplesPort = pingPacket->pingChanHeader[0].NumSamples;
            pingPacket->p_ping[0] = malloc(bytesPerSamplePort * numSamplesPort);
            memset(pingPacket->p_ping[0], 0, bytesPerSamplePort*numSamplesPort);
            m_file.read((char *)pingPacket->p_ping[0], bytesPerSamplePort*numSamplesPort);
            //read the chanheader of channel 1
            m_file.read((char *)&pingPacket->pingChanHeader[1], sizeof(xtf_pingchanheader));
            //read the data of channel 1
            unsigned short bytesPerSampleStbd = fileHeader.chaninfo[pingPacket->pingChanHeader[1].ChannelNumber].BytesPerSample;
            unsigned int numSamplesStbd = pingPacket->pingChanHeader[1].NumSamples;
            pingPacket->p_ping[1] = malloc(bytesPerSampleStbd * numSamplesStbd);
            memset(pingPacket->p_ping[1], 0, bytesPerSampleStbd*numSamplesStbd);
            m_file.read((char *)pingPacket->p_ping[1], bytesPerSampleStbd*numSamplesStbd);

            //read the next ping
            tState = transHaveFileHeader;

            //trans sidescan data to frame and pop to the queue
            frameQueueMutex.lock();
            frameQueue.push(new pingFrame(pingPacket, fileHeader));
            frameQueueMutex.unlock();
            pingPacket->p_ping[0] = NULL;
            pingPacket->p_ping[1] = NULL;
            delete pingPacket;
            //usleep(40000);
        }
        
        //read the bathmetry data
        if(tState == transHaveBathymetry){
            xtf_bathmetrypacket *bathymetryPacket = new xtf_bathmetrypacket;
            memset(bathymetryPacket, 0, sizeof(xtf_bathmetrypacket));
            //load the pingheader
            bathymetryPacket->bathmetryHeader.packetHeader = packetHeader;
            m_file.read((char*)&bathymetryPacket->bathmetryHeader+sizeof(xtf_packetheader), sizeof(xtf_pingheader) - sizeof(xtf_packetheader));
                        
            //load the bathmetry data
            int bathymetryChannelNum = 0;
            for(bathymetryChannelNum = 0; bathymetryChannelNum < 6; bathymetryChannelNum++){
                if(int(fileHeader.chaninfo[bathymetryChannelNum].TypeOfChannel) == 3){
                    bathymetryPacket->bathymetryChannelNum = bathymetryChannelNum;
                    break;
                }
            }
            unsigned short bytesPerSample = fileHeader.chaninfo[bathymetryPacket->bathymetryChannelNum].BytesPerSample;
            unsigned int numBytes = packetHeader.NumBytesThisRecord;
            bathymetryPacket->p_bathmetry = malloc(numBytes - sizeof(xtf_pingheader));
            memset(bathymetryPacket->p_bathmetry, 0, numBytes - sizeof(xtf_pingheader));
            m_file.read((char*)bathymetryPacket->p_bathmetry, numBytes - sizeof(xtf_pingheader));
            
            //read the next ping
            tState = transHaveFileHeader;
            
            //trans bathymetry data to frame and pop to the queue
            frameQueueMutex.lock();
            frameQueue.push(new bathmetryFrame(bathymetryPacket, fileHeader));
            frameQueueMutex.unlock();

            bathymetryPacket->p_bathmetry = NULL;
            delete bathymetryPacket; 
        }
        
        //read the attitude data
        if(tState == transHaveAttitude){
            xtf_attitudeheader *attitudePacket = new xtf_attitudeheader;
            memset(attitudePacket, 0, sizeof(xtf_attitudeheader));
            attitudePacket->packetHeader = packetHeader;
            m_file.read((char*)&attitudePacket + sizeof(xtf_packetheader), sizeof(xtf_attitudeheader) - sizeof(xtf_packetheader));

            //read the next ping
            tState = transHaveFileHeader;
            
            //trans attitude data to frame and pop to the queue
            frameQueueMutex.lock();
            frameQueue.push(new attiFrame(attitudePacket));
            frameQueueMutex.unlock();
            delete attitudePacket;
        }

        //finish reading the xtf file
        if(m_file.eof()){
            if(m_file.is_open()){
                m_file.close();
            }
            sleep(0.5);
            tState = transStop;
            transSwitch = false;
        }            
    }
    return 0;
}

int xtfDataAccess::fetchFrame(Frame * &pFrame){
    if(frameQueue.size() >= 1){
        frameQueueMutex.lock();
        pFrame = frameQueue.front();
        frameQueue.pop();
        frameQueueMutex.unlock();
        return 1;
    }
    return 0;
}

