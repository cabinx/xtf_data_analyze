#include "DataExport/folderPathProcess.h"

folderPathProcess::folderPathProcess(){}

folderPathProcess::~folderPathProcess(){}

int folderPathProcess::getFileNames(string path, vector<string>& filenames){
    DIR *pDir;
    struct dirent* ptr;
    if(!(pDir = opendir(path.c_str()))){
        std::cout<<"Folder doesn't exist!!!"<<std::endl;
        return 1;
    }
    while((ptr = readdir(pDir)) != 0){
        if(strcmp(ptr->d_name, ".") != 0 && strcmp(ptr->d_name, "..") != 0){
            filenames.push_back(path+ "/" + ptr->d_name);
        }
    }
    closedir(pDir);
    return 0;
}