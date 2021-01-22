/*-------------------------------------------------------------------
* process while the input is a folder path
* Organization: PengCheng Lab
* Author: cabin
* Date: jan 8, 2021
--------------------------------------------------------------------*/
#ifndef FOLDER_PATH_PROCESS_H_
#define FOLDER_PATH_PROCESS_H_
#include <iostream>
#include <cstring>
#include <vector>
#include <dirent.h>
using namespace std;
class folderPathProcess{
public:
    folderPathProcess();
    ~folderPathProcess();

    int getFileNames(string path, vector<string>& filenames);
};
#endif