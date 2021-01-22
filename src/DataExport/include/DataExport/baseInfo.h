/*-------------------------------------------------------------------
* the base information struct of a xtf file
* Organization: PengCheng Lab
* Author: cabin
* Date: Dec 31, 2020
--------------------------------------------------------------------*/
#ifndef  BASE_INFO_H
#define  BASE_INFO_H
#include <ctime>
#include "DataAccess/xtf.h"

struct baseInfo{
    struct xtf_fileheader fileHeader;
    double startTime;
    double endTime;
    double startLon;
    double startLat;
    double endLon;
    double endLat;
    int pingNum;
};
#endif    