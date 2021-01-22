/*-------------------------------------------------------------------
* export the base info to json file, please refer to "Triton Imaging, Inc. eXtended Triton Format(XTF) Rev.41" for more information
* thanks to the rapidjson(a fast json parser/generator for c++ with both SAX/DOM style API) for json file processing, for more information: http://rapidjson.org/
* the format is the following:
*{
    "BaseInfo":
    {
        "FileName" : xxx ,
        "SonarName" : xxx ,
        "TotalPingNumber" : xxx ,
        "NavUnits" : xxx ,
        "NumberOfSonarChannels" : xxx,
        "NumberOfBathymetryChannels: : xxx        
    },
    "FirstPing":
    {
        "Time" : xxx,
        "Longitude" : xxx,
        "Latitude" : xxx
    },
    "LastPing":
    {
        "Time" : xxx,
        "Longitude" : xxx,
        "Latitude" : xxx
    },
    "chan_x":
    {
        "TypeOfChannel" : xxx,
        "SubChannelNumber" : xxx,
        "CorrectionFlags" : xxx,
        "BytesPerSample" : xxx,
        "VoltScale" : xxx,
        "Frequency" : xxx,
        "HorizBeamAngle" : xxx,
        "TiltAngle" : xxx,
        "BeamWidth" : xxx
    }

}
* Organization: PengCheng Lab
* Author: cabin
* Date: Dec 31, 2020
--------------------------------------------------------------------*/
#ifndef BASE_INFO_DATA_EXPORT_H_
#define BASE_INFO_DATA_EXPORT_H_
#include <fstream>
#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "DataExport/baseInfo.h"
using namespace std;
class baseInfoDataExport{
public:
    baseInfoDataExport();
    ~baseInfoDataExport();

    int writeToJsonFile(string xtfFilePath, struct baseInfo baseInfos);

private:
    string baseInfoFilePath;
    rapidjson::Document mDocument;
    string startDT;
    string endDT;
};
#endif