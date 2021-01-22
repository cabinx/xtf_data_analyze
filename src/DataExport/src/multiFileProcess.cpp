/*-------------------------------------------------------------------
* process a folder with sidescan xtf files, the output info in the singleFileProcess.h
* Organization: PengCheng Lab
* Author: cabin
* Date: Jan 8, 202120
--------------------------------------------------------------------*/
#include "DataExport/loadXtfFile.h"
#include "DataExport/baseInfoDataExport.h"
#include "DataExport/trajectoryDataExport.h"
#include "DataExport/intensityDataExport.h"
#include "DataExport/folderPathProcess.h"
int main(int argc, char **argv){
    folderPathProcess folderProcess;
    
    string folderPath = argv[1];
    vector<string> filenames;
    int folderStates = folderProcess.getFileNames(folderPath, filenames);
    
    if(!folderStates){
        for(int i = 0; i < filenames.size(); i++){
            string currentFile = filenames[i];
            loadXtfFile xtfData;
            int loadState = xtfData.loadData(currentFile);
            
            //remove the json and mat files
            int pos = currentFile.find_last_of('.');
            string fileSuffix = currentFile.substr(pos+1);
            int suffixState = 0;
            if(fileSuffix == "json" || fileSuffix == "mat"){
                suffixState = 1;
            }

            if(!loadState && !suffixState){
                baseInfoDataExport exportBaseInfo;
                exportBaseInfo.writeToJsonFile(currentFile, xtfData.baseInfos);
    
                //write the trajectroy with timestamp data    
                trajectoryDataExport exportTraj;
                exportTraj.loadInitData(xtfData.totalPingNum);
                exportTraj.writeToMatFile(currentFile, xtfData.trajectoryMatrix);

                //write the intensity data
                intensityDataExport exportIntensity;
                exportIntensity.loadInitData(xtfData.totalPingNum, xtfData.numSamplesPort, xtfData.numSamplesStbd);
                exportIntensity.writeToMatFile(currentFile, xtfData.portMatrix, xtfData.stbdMatrix);
            }
        }
        return 0;
    }
    else{
        std::cout<<"loading folder path error!!!"<<std::endl;
        return 1;
    }
}