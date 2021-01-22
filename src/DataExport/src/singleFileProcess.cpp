/*-------------------------------------------------------------------
* process a sidescan xtf file, export the basic info to a json file, the trajectory info to a mat file(xxx_TRAJ.mat), the port side intensity to a mat file(xxx_PIM.mat),
* the stbd side intensity to a mat file(xxx_SIM.mat)
* Organization: PengCheng Lab
* Author: cabin
* Date: Dec 28, 2020
--------------------------------------------------------------------*/
#include "DataExport/loadXtfFile.h"
#include "DataExport/baseInfoDataExport.h"
#include "DataExport/trajectoryDataExport.h"
#include "DataExport/intensityDataExport.h"
int main(int argc, char **argv){
    //get the xtf file path
    string xtfFilePath = argv[1];
    
    //load the xtf file data
    loadXtfFile xtfData;
    int loadState = xtfData.loadData(xtfFilePath);

    if(!loadState){
        //write the base info
        baseInfoDataExport exportBaseInfo;
        exportBaseInfo.writeToJsonFile(xtfFilePath, xtfData.baseInfos);
        
        //write the trajectroy with timestamp data    
        trajectoryDataExport exportTraj;
        exportTraj.loadInitData(xtfData.totalPingNum);
        exportTraj.writeToMatFile(xtfFilePath, xtfData.trajectoryMatrix);

        //write the intensity data
        intensityDataExport exportIntensity;
        exportIntensity.loadInitData(xtfData.totalPingNum, xtfData.numSamplesPort, xtfData.numSamplesStbd);
        exportIntensity.writeToMatFile(xtfFilePath, xtfData.portMatrix, xtfData.stbdMatrix);
    
        return 0;
    }
    std::cout<<"current file is "<<xtfFilePath<<std::endl;
    std::cout<<"load xtf file error!!!"<<std::endl;
    return 1;
}