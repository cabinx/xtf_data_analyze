#include "DataExport/intensityDataExport.h"

intensityDataExport::intensityDataExport(){
    totalPingNum = 0;
}

intensityDataExport::~intensityDataExport(){}

int intensityDataExport::loadInitData(int tPingNum, int nSamplesPort, int nSamplesStbd){
    totalPingNum = tPingNum;
    numSamplesPort = nSamplesPort;
    numSamplesStbd = nSamplesStbd;
    return 0;
}

int intensityDataExport::writeToMatFile(string xtfFilePath, vector<vector<double>> &portMatrix, vector<vector<double>> &stbdMatrix){
    //create the mat files path according to the xtf file path
    int pos = xtfFilePath.find_last_of('.'); 
    pimFilePath = xtfFilePath.substr(0,pos)+"_PIM.mat";
    simFilePath = xtfFilePath.substr(0,pos)+"_SIM.mat";
    
    //the port side
    size_t dimsPort[2] = {totalPingNum, numSamplesPort};
    mat_t *matfpPort = NULL;
    matvar_t *matvarPort;
    matfpPort = Mat_Open(pimFilePath.c_str(), MAT_ACC_RDWR);

    //creat the mat file if not exist
    if(!matfpPort){
        matfpPort = Mat_CreateVer(pimFilePath.c_str(), NULL, MAT_FT_MAT5);
        Mat_Close(matfpPort);
        matfpPort = Mat_Open(pimFilePath.c_str(), MAT_ACC_RDWR);
    }
    else{
        Mat_VarDelete(matfpPort, "port_intensity_matrix");
    }
    matrixTransForMatio(portMatrix, totalPingNum, numSamplesPort);

    double *portM;
    portM = malloc(sizeof(double)*(totalPingNum * numSamplesPort));
    for(int i = 0; i < totalPingNum; i++){
        for(int j = 0; j < numSamplesPort; j++){
            portM[i* numSamplesPort + j] = portMatrix[i][j];
        }
    }

    matvarPort = Mat_VarCreate("port_intensity_matrix", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dimsPort, portM, 0);
    Mat_VarWrite(matfpPort, matvarPort, MAT_COMPRESSION_ZLIB);
    Mat_VarFree(matvarPort);
    Mat_Close(matfpPort);
    free(portM);
    //end of the port side
    
    //the starboard side
    size_t dimsStbd[2] = {totalPingNum, numSamplesStbd};
    mat_t *matfpStbd = NULL;
    matvar_t *matvarStbd;
    matfpStbd = Mat_Open(simFilePath.c_str(), MAT_ACC_RDWR);

    //creat the mat file if not exist
    if(!matfpStbd){
        matfpStbd = Mat_CreateVer(simFilePath.c_str(), NULL, MAT_FT_MAT5);
        Mat_Close(matfpStbd);
        matfpStbd = Mat_Open(simFilePath.c_str(), MAT_ACC_RDWR);
    }
    else{
        Mat_VarDelete(matfpStbd, "stbd_intensity_matrix");
    }
    matrixTransForMatio(stbdMatrix, totalPingNum, numSamplesStbd);
    
    double *stbdM;
    stbdM = malloc(sizeof(double)*(totalPingNum * numSamplesStbd));
    for(int i = 0; i < totalPingNum; i++){
        for(int j = 0; j < numSamplesStbd; j++){
            stbdM[i* numSamplesStbd + j] = stbdMatrix[i][j];
        }
    }
    
    matvarStbd = Mat_VarCreate("stbd_intensity_matrix", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dimsStbd, stbdM, 0);
    Mat_VarWrite(matfpStbd, matvarStbd, MAT_COMPRESSION_ZLIB);
    Mat_VarFree(matvarStbd);
    Mat_Close(matfpStbd);
    free(stbdM);
    return 0;
}

int intensityDataExport::matrixTransForMatio(vector<vector<double>> &matrixIn, int row, int col){
    vector<vector<double>> matrixTrans;
    vector<double> mTrans;
    for(int i = 0; i < col; i ++){
        for(int j = 0; j < row; j++){
            mTrans.push_back(matrixIn[j][i]);
        }
        matrixTrans.push_back(mTrans);
        vector<double>().swap(mTrans);
    }

    vector<vector<double>>().swap(matrixIn);
    
    vector<double> matrixRow;
    for(int i = 0; i < row; i++){
        for(int j = 0; j<col; j++){
            int curNum = i * col + j;
            int iTrans = curNum / row;
            int jTrans = curNum % row;
            matrixRow.push_back(matrixTrans[iTrans][jTrans]);
        }
        matrixIn.push_back(matrixRow);
        vector<double>().swap(matrixRow);
    }
    vector<vector<double>>().swap(matrixTrans);
    return 0;
}
