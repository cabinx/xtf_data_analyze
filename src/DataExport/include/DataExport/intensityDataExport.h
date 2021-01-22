/*-------------------------------------------------------------------
* export the intensity matrix data of port and stardboard side of sidescan to mat files
* Organization: PengCheng Lab
* Author: cabin
* Date: Dec 21, 2020
--------------------------------------------------------------------*/
#ifndef INTENSITY_DATA_EXPORT_H_
#define INTENSITY_DATA_EXPORT_H_
#include <string>
#include <vector>
#include <matio.h>

using namespace std;
class intensityDataExport{
public:
    intensityDataExport();
    ~intensityDataExport();
    int loadInitData(int tPingNum, int nSamplesPort, int nSamplesStbd);
    int writeToMatFile(string xtfFilePath, vector<vector<double>> &portMatrix, vector<vector<double>> &stbdMatrix);
    
private:
    int matrixTransForMatio(vector<vector<double>> &matrixIn, int row, int col);    //trans the input matrix of matio, for exporting the correct order to the mat file
    string pimFilePath;       //port side intensity mat file path
    string simFilePath;       //stbd side intensity mat file path    
    int totalPingNum;         //the total ping numbers of the xtf file recorded (the row of the matrix)
    int numSamplesPort;       //the number samples of port side (the col of the matrix)
    int numSamplesStbd;       //the number samples of starboard side (the col of the matrix)
};
#endif