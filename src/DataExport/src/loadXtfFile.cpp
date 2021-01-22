#include "DataExport/loadXtfFile.h"

loadXtfFile::loadXtfFile(){
    totalPingNum = 0;
}

loadXtfFile::~loadXtfFile(){}

int loadXtfFile::loadData(string xtfFilePath){
    //load the data
    xtfDataAccess loadData;
    loadData.xtfFilePath(xtfFilePath);
    int loadState = loadData.xtfPingTrans();
    //arrange the data
    if(!loadState){
        Frame *pFrame;
        pingFrame *curFrame;
    
        baseInfos.fileHeader = loadData.fileHeader;

        int fetchState = 0;
        fetchState = loadData.fetchFrame(pFrame);
        bool firstPing = true;
        //time_t endPingTime;
        while(fetchState){
            curFrame = (pingFrame *)pFrame;
            vector<double> trajData;
            vector<double> pData;
            vector<double> sData;
            if(pFrame->packetType == XTF_DATA_SIDESCAN){
                totalPingNum++;
                if(firstPing){
                    baseInfos.startTime = curFrame->timeStamp;
                    baseInfos.startLon =  curFrame->sensorXcoordinate;
                    baseInfos.startLat = curFrame->sensorYcoordinate;
                    firstPing = false;
                }
                else{
                    baseInfos.endTime = curFrame->timeStamp;
                    baseInfos.endLon = curFrame->sensorXcoordinate;
                    baseInfos.endLat = curFrame->sensorYcoordinate;
                }
                //loads the trajectory data with timestamp
                trajData.push_back(curFrame->timeStamp);
                trajData.push_back(curFrame->sensorXcoordinate);
                trajData.push_back(curFrame->sensorYcoordinate);

                trajData.push_back(curFrame->sensorPrimaryAltitude);
                trajData.push_back(curFrame->slantRangePort);
                trajData.push_back(curFrame->slantRangeStbd);
                trajData.push_back(curFrame->numSamplesPort);
                trajData.push_back(curFrame->numSamplesStbd);
                trajData.push_back(curFrame->sensorSpeed);
                trajData.push_back(curFrame->sensorPitch);
                trajData.push_back(curFrame->sensorRoll);
                trajData.push_back(curFrame->sensorHeading);
            
                trajectoryMatrix.push_back(trajData);

                //loads the port side intensity data
                for(int i = 0; i < curFrame->numSamplesPort; i++){
                    if(curFrame->bytesPerSamplePort == 1){
                        pData.push_back(*((unsigned char *)(curFrame->p_data[0]) + i));
                    }
                    else if(curFrame->bytesPerSamplePort == 2){
                        pData.push_back(*((unsigned short *)(curFrame->p_data[0]) + i));
                    }
                    else{
                        std::cout<<"bytes PerSample Port error!!!"<<std::endl;
                        return 1;
                    }
                }
                portMatrix.push_back(pData);

                //loads the stbd side intensity data
                for(int i = 0; i < curFrame->numSamplesStbd; i++){
                    if(curFrame->bytesPerSampleStbd == 1){
                        sData.push_back(*((unsigned char *)(curFrame->p_data[1]) + i)); 
                    }
                    else if(curFrame->bytesPerSampleStbd == 2){
                        sData.push_back(*((unsigned short *)(curFrame->p_data[1]) + i)); 
                    }
                    else{
                        std::cout<<"bytes PerSample Stbd error!!!"<<std::endl;
                        return 1;
                    }
                }
                stbdMatrix.push_back(sData);
            }
            fetchState = loadData.fetchFrame(pFrame);       
        }
        baseInfos.pingNum = totalPingNum;
    
        numSamplesPort = int(curFrame->numSamplesPort);
        numSamplesStbd = int(curFrame->numSamplesStbd);
        return 0;
    }
    return 2;
}