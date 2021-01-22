/*-------------------------------------------------------------------
* process the binary stream of xtf file with pointer and forced type conversion
* Organization: PengCheng Lab
* Author: cabin
* Date: Dec 17, 2020
--------------------------------------------------------------------*/
#ifndef XTFDATAACCESS_H_
#define XTFDATAACCESS_H_
#include <queue>
#include <cstring>
#include <vector>
#include <mutex>
#include <thread>
#include <functional>
#include <fstream>
#include <unistd.h>
#include <iomanip>

#include "xtf.h"
#include "frame.h"

using namespace std;
enum transState{transInit, transHaveFileHeader, transHavePing, transHaveBathymetry, transHaveAttitude, transStop};
class xtfDataAccess{
public:
    xtfDataAccess();
    ~xtfDataAccess();

    struct xtf_fileheader fileHeader;
    int xtfPingTrans();   //trans the xtf stream data
    int xtfFilePath(string filepath);
    int fetchFrame(Frame * &pFrame);     //fetch a frame from the queue
    

private:
    //int channelNum;
    //int curPingNum;
    bool transSwitch;
    transState tState;
    std::string m_filePath;
    std::fstream m_file;
    static std::mutex frameQueueMutex;
    std::queue<Frame *> frameQueue;      //a queue stores frames, one frame trans from one ping data 
};
#endif