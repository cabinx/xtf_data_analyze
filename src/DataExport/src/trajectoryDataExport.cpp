#include "DataExport/trajectoryDataExport.h"

trajectoryDataExport::trajectoryDataExport(){
    totalPingNum = 0;
}

trajectoryDataExport::~trajectoryDataExport(){}

int trajectoryDataExport::loadInitData(int tPingNum){
    totalPingNum = tPingNum;
}

int trajectoryDataExport::matrixTransForMatio(vector<vector<double>> &matrixIn, int row, int col){
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

int trajectoryDataExport::writeToMatFile(string xtfFilePath, vector<vector<double>> &trajMatrix){
    //create the mat files path according to the xtf file path
    int pos = xtfFilePath.find_last_of('.'); 
    trajFilePath = xtfFilePath.substr(0,pos)+"_TRAJ.mat";

    int trajMatrixCol = trajMatrix[0].size();
    
    size_t dimsTraj[2] = {totalPingNum, trajMatrixCol};
    mat_t *matfpTraj = NULL;
    matvar_t *matvarTraj;
    matfpTraj = Mat_Open(trajFilePath.c_str(), MAT_ACC_RDWR);

    //creat the mat file if not exist
    if(!matfpTraj){
        matfpTraj = Mat_CreateVer(trajFilePath.c_str(), NULL, MAT_FT_MAT5);
        Mat_Close(matfpTraj);
        matfpTraj = Mat_Open(trajFilePath.c_str(), MAT_ACC_RDWR);
    }
    else{
        Mat_VarDelete(matfpTraj, "traj_matrix");
    }

    matrixTransForMatio(trajMatrix, totalPingNum, trajMatrixCol);

    double *trajM;
    trajM = malloc(sizeof(double)*(totalPingNum * trajMatrixCol));
    for(int i = 0; i < totalPingNum; i++){
        for(int j = 0; j < trajMatrixCol; j++){
            trajM[i* trajMatrixCol + j] = trajMatrix[i][j];
        }
    }

    matvarTraj = Mat_VarCreate("traj_matrix", MAT_C_DOUBLE, MAT_T_DOUBLE, 2, dimsTraj, trajM, 0);
    Mat_VarWrite(matfpTraj, matvarTraj, MAT_COMPRESSION_ZLIB);
    Mat_VarFree(matvarTraj);
    Mat_Close(matfpTraj);
    free(trajM);

    return 0;
}