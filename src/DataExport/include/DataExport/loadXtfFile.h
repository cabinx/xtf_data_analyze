/*-------------------------------------------------------------------
* load the xtf file data and arrange to the classes to export 
* Organization: PengCheng Lab
* Author: cabin
* Date: Dec 28, 2020
--------------------------------------------------------------------*/
#ifndef LOAD_XTF_FILE_H_
#define LOAD_XTF_FILE_H_
#include "DataExport/baseInfo.h"
#include "DataAccess/xtfDataAccess.h"
class loadXtfFile{
public:
    loadXtfFile();
    ~loadXtfFile();
    int loadData(string xtfFilePath);
    
    //for base info data
    struct baseInfo baseInfos;
    
    //for trajectory data
    //double timestamp;
    //double trajectoryX;       //longitude or easting of the sidescan
    //double trajectoryY;       //latitude or northing of the sidescan
    vector<vector<double>> trajectoryMatrix;     //the trajectory matrix with timestamp
    
    //for intensity data
    int totalPingNum;         //the total ping numbers of the xtf file recorded (the row of the matrix)
    int numSamplesPort;       //the number samples of port side (the col of the matrix)
    int numSamplesStbd;       //the number samples of starboard side (the col of the matrix)
    vector<vector<double>> portMatrix;     //the port side intensity matrix data
    vector<vector<double>> stbdMatrix;     //the starboard side intensity matrix data

};
#endif