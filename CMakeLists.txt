cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

project(XTF_DATA_ANALYZE_Project)

set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_CXX_FLAGS "-std=c++14 -lpthread")
#set(CMAKE_CXX_FLAGS "-fpermissive")
set(CMAKE_CXX_FLAGS "-std=c++14 -pthread -fpermissive")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -Wall -g")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -Wall -g -ggdb")
set (CMAKE_VERBOSE_MAKEFILE ON)

message(WARNING "${PROJECT_SOURCE_DIR}")





include_directories( ${OpenCV_INCLUDE_DIRS} )


include_directories(${PROJECT_SOURCE_DIR}/src/DataAccess/include
                    ${PROJECT_SOURCE_DIR}/src/DataExport/include) 

include_directories(/usr/local/include)
link_directories(${PROJECT_SOURCE_DIR}/build)



add_executable (singleFileProcess ${PROJECT_SOURCE_DIR}/src/DataExport/src/singleFileProcess.cpp
                                  ${PROJECT_SOURCE_DIR}/src/DataExport/src/loadXtfFile.cpp 
                                  ${PROJECT_SOURCE_DIR}/src/DataExport/src/intensityDataExport.cpp
                                  ${PROJECT_SOURCE_DIR}/src/DataExport/src/trajectoryDataExport.cpp
                                  ${PROJECT_SOURCE_DIR}/src/DataExport/src/baseInfoDataExport.cpp
                                  ${PROJECT_SOURCE_DIR}/src/DataAccess/src/xtfDataAccess.cpp
                                  ${PROJECT_SOURCE_DIR}/src/DataAccess/src/frame.cpp
                                  ${PROJECT_SOURCE_DIR}/src/DataAccess/src/logger.cpp)
target_link_libraries(singleFileProcess /usr/lib/x86_64-linux-gnu/libmatio.so)

add_executable (multiFileProcess ${PROJECT_SOURCE_DIR}/src/DataExport/src/multiFileProcess.cpp
                                 ${PROJECT_SOURCE_DIR}/src/DataExport/src/folderPathProcess.cpp
                                 ${PROJECT_SOURCE_DIR}/src/DataExport/src/loadXtfFile.cpp 
                                  ${PROJECT_SOURCE_DIR}/src/DataExport/src/intensityDataExport.cpp
                                  ${PROJECT_SOURCE_DIR}/src/DataExport/src/trajectoryDataExport.cpp
                                  ${PROJECT_SOURCE_DIR}/src/DataExport/src/baseInfoDataExport.cpp
                                  ${PROJECT_SOURCE_DIR}/src/DataAccess/src/xtfDataAccess.cpp
                                  ${PROJECT_SOURCE_DIR}/src/DataAccess/src/frame.cpp
                                  ${PROJECT_SOURCE_DIR}/src/DataAccess/src/logger.cpp)
target_link_libraries(multiFileProcess /usr/lib/x86_64-linux-gnu/libmatio.so)

