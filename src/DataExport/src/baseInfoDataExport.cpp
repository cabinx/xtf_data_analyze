#include "DataExport/baseInfoDataExport.h"

baseInfoDataExport::baseInfoDataExport(){}

baseInfoDataExport::~baseInfoDataExport(){}

/*
int baseInfoDataExport::createJsonFile(string xtfFilePath){
    std::fstream mFile;
    int pos = xtfFilePath.find_last_of('.');
    baseInfoFilePath = xtfFilePath.substr(0, pos) + "_info.json";
    mFile.open(baseInfoFilePath, fstream::out);
    mFile.close();
    return 0;
}
*/

int baseInfoDataExport::writeToJsonFile(string xtfFilePath, struct baseInfo baseInfos){
    //create json file
    std::fstream mFile;
    int pos = xtfFilePath.find_last_of('.');
    baseInfoFilePath = xtfFilePath.substr(0, pos) + "_info.json";
    mFile.open(baseInfoFilePath, fstream::out);
    mFile.close();

    int posName = xtfFilePath.find_last_of('/');
    string fileName = xtfFilePath.substr(posName + 1);
    
    //the first ping time
    struct tm startTimeTM;    
    time_t startTimeT = baseInfos.startTime;    
    localtime_r(&startTimeT, &(startTimeTM));    
    int startHseconds = int((baseInfos.startTime-(long long)(baseInfos.startTime))*100);
    string startDate = std::to_string(startTimeTM.tm_year) +"." + std::to_string(startTimeTM.tm_mon) + "." + std::to_string(startTimeTM.tm_mday);
    string startTime = std::to_string(startTimeTM.tm_hour) + ":" + std::to_string(startTimeTM.tm_min) + ":" + std::to_string(startTimeTM.tm_sec)+"."+std::to_string(startHseconds);
    string startDT = startDate + "  " + startTime;
    
    //the last ping time
    struct tm endTimeTM;
    time_t endTimeT = baseInfos.endTime;
    localtime_r(&endTimeT, &(endTimeTM));
    int endHseconds = int((baseInfos.endTime-(long long)(baseInfos.endTime))*100);
    string endDate = std::to_string(endTimeTM.tm_year) +"." + std::to_string(endTimeTM.tm_mon) + "." + std::to_string(endTimeTM.tm_mday);
    string endTime = std::to_string(endTimeTM.tm_hour) + ":" + std::to_string(endTimeTM.tm_min) + ":" + std::to_string(endTimeTM.tm_sec)+"."+std::to_string(endHseconds);
    string endDT = endDate + "  " + endTime;
    
    //creat rapidjson value
    mDocument.SetObject();
    
    //base info
    rapidjson::Value jsonBaseInfo(rapidjson::kObjectType);
    
    rapidjson::Value jsonXtfFileName;
    jsonXtfFileName.SetString(fileName.c_str(), strlen(fileName.c_str()), mDocument.GetAllocator());
    jsonBaseInfo.AddMember("FileName", jsonXtfFileName, mDocument.GetAllocator());

    rapidjson::Value jsonSonarName;
    jsonSonarName.SetString(baseInfos.fileHeader.SonarName, strlen(baseInfos.fileHeader.SonarName), mDocument.GetAllocator());
    jsonBaseInfo.AddMember("SonarName", jsonSonarName, mDocument.GetAllocator());

    rapidjson::Value jsonPingNumber;
    jsonPingNumber.SetInt(baseInfos.pingNum);
    jsonBaseInfo.AddMember("TotalPingNumber", jsonPingNumber, mDocument.GetAllocator());

    rapidjson::Value jsonNavUnits;
    jsonNavUnits.SetInt(baseInfos.fileHeader.NavUnits);
    jsonBaseInfo.AddMember("NavUnits", jsonNavUnits, mDocument.GetAllocator());

    rapidjson::Value jsonNumOfSonarChan;
    jsonNumOfSonarChan.SetInt(baseInfos.fileHeader.NumberOfSonarChannels);
    jsonBaseInfo.AddMember("NumberOfSonarChannels", jsonNumOfSonarChan, mDocument.GetAllocator());

    rapidjson::Value jsonNumOfBathymetryChan;
    jsonNumOfBathymetryChan.SetInt(baseInfos.fileHeader.NumberOfBathymetryChannels);
    jsonBaseInfo.AddMember("NumberOfBathymetryChannels", jsonNumOfBathymetryChan, mDocument.GetAllocator());
    
    mDocument.AddMember("BaseInfo", jsonBaseInfo, mDocument.GetAllocator());
    
    //first ping
    rapidjson::Value jsonFirstPing(rapidjson::kObjectType);

    rapidjson::Value jsonStartTime;
    jsonStartTime.SetString(startDT.c_str(), strlen(startDT.c_str()), mDocument.GetAllocator());
    jsonFirstPing.AddMember("Time", jsonStartTime, mDocument.GetAllocator());

    rapidjson::Value jsonStartLon;
    jsonStartLon.SetDouble(baseInfos.startLon);
    jsonFirstPing.AddMember("Longitude", jsonStartLon, mDocument.GetAllocator());

    rapidjson::Value jsonStartLat;
    jsonStartLat.SetDouble(baseInfos.startLat);
    jsonFirstPing.AddMember("Latitude", jsonStartLat, mDocument.GetAllocator());

    mDocument.AddMember("FirstPing", jsonFirstPing, mDocument.GetAllocator());

    //last ping
    rapidjson::Value jsonLastPing(rapidjson::kObjectType);

    rapidjson::Value jsonEndTime;
    jsonEndTime.SetString(endDT.c_str(), strlen(endDT.c_str()), mDocument.GetAllocator());
    jsonLastPing.AddMember("Time", jsonEndTime, mDocument.GetAllocator());

    rapidjson::Value jsonEndLon;
    jsonEndLon.SetDouble(baseInfos.endLon);
    jsonLastPing.AddMember("Longitude", jsonEndLon, mDocument.GetAllocator());

    rapidjson::Value jsonEndLat;
    jsonEndLat.SetDouble(baseInfos.endLat);
    jsonLastPing.AddMember("Latitude", jsonEndLat, mDocument.GetAllocator());

    mDocument.AddMember("LastPing", jsonLastPing, mDocument.GetAllocator());

    //channel info
    for(int i = 0; i < baseInfos.fileHeader.NumberOfSonarChannels + baseInfos.fileHeader.NumberOfBathymetryChannels; i++){
        //rapidjson::Value jsonTypeOfChan;
        rapidjson::Value jsonChanInfo(rapidjson::kObjectType);
        jsonChanInfo.AddMember("TypeOfChannel", baseInfos.fileHeader.chaninfo[i].TypeOfChannel, mDocument.GetAllocator());
        jsonChanInfo.AddMember("SubChannelNumber", baseInfos.fileHeader.chaninfo[i].SubChannelNumber, mDocument.GetAllocator());
        jsonChanInfo.AddMember("CorrectionFlags", baseInfos.fileHeader.chaninfo[i].CorrectionFlags, mDocument.GetAllocator());
        jsonChanInfo.AddMember("BytesPerSample", baseInfos.fileHeader.chaninfo[i].BytesPerSample, mDocument.GetAllocator());
        jsonChanInfo.AddMember("VoltScale", baseInfos.fileHeader.chaninfo[i].VoltScale, mDocument.GetAllocator());
        jsonChanInfo.AddMember("Frequency", baseInfos.fileHeader.chaninfo[i].Frequency, mDocument.GetAllocator());
        jsonChanInfo.AddMember("HorizBeamAngle", baseInfos.fileHeader.chaninfo[i].HorizBeamAngle, mDocument.GetAllocator());
        jsonChanInfo.AddMember("TiltAngle", baseInfos.fileHeader.chaninfo[i].TiltAngle, mDocument.GetAllocator());
        jsonChanInfo.AddMember("BeamWidth", baseInfos.fileHeader.chaninfo[i].BeamWidth, mDocument.GetAllocator());
        string keyName =  "chan_" + std::to_string(i);
        rapidjson::Value jsonKeyName;
        jsonKeyName.SetString(keyName.c_str(), strlen(keyName.c_str()), mDocument.GetAllocator());
        mDocument.AddMember(jsonKeyName, jsonChanInfo, mDocument.GetAllocator());
    }

    //write rapidjson value to json file
    rapidjson::StringBuffer infoBuffer;
    rapidjson::Writer<rapidjson::StringBuffer>  infoWriter(infoBuffer);
    mDocument.Accept(infoWriter);
    const char* infoStr = infoBuffer.GetString();
    FILE *infoFile = fopen(baseInfoFilePath.c_str(), "w");
    fwrite(infoStr, sizeof(char), strlen(infoStr), infoFile);
    fclose(infoFile);

    return 0;
    
}
