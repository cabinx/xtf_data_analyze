/*-------------------------------------------------------------------
* export the trajectory data to mat files, one ping data is recorded in the following format : 
[timestamp, longitude, latitude, altitude, slantRangePort, slantRangeStbd, numSamplesPort, numSamplesStbd, sensorSpeed, sensorPitch, sensorRoll, sensorHeading]
* Organization: PengCheng Lab
* Author: cabin
* Date: Dec 21, 2020
--------------------------------------------------------------------*/
#ifndef TRAJECTORY_DATA_EXPORT
#define TRAJECTORY_DATA_EXPORT
#include <string>
#include <vector>
#include <matio.h>
using namespace std;
class trajectoryDataExport{
public:
    trajectoryDataExport();
    ~trajectoryDataExport();
    int loadInitData(int tPingNum);
    int writeToMatFile(string xtfFilePath, vector<vector<double>> &trajMatrix);

private:
    int matrixTransForMatio(vector<vector<double>> &matrixIn, int row, int col);
    int totalPingNum;
    string trajFilePath;       //trajectory mat file path
};
#endif