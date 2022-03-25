## 2022.03.23 
## created by Kelly (ekma@4dreplay.com)
##
cmake_minimum_required(VERSION 3.7)
project(zion)
set (PLATFORM LINUX) # available : MACM1, MACINTEL, WIN, ARM, LINUX
message(${PLATFORM})
set (GPUMODE off )

if( ${PLATFORM} STREQUAL "WIN" )
#    set(OpenCV_INCLUDE_DIRS ${PRJ_PATH}/3rd/opencv4.5.3/include/)
#    set(OpenCV_LIBS ${PRJ_PATH}/3rd/opencv4.5.3/lib/)
else()
    find_package( OpenCV REQUIRED )
endif()

set(CMAKE_CXX_STANDARD 17)
if( ${PLATFORM} STREQUAL "LINUX" )
    set(CMAKE_CXX_FLAGS "-Wno-parentheses -pthread -I_3rdparty_/ -Icommon/ -Icalib/ -Idaemon/ -Iendecode -Imanager/ -Istabil -O2 -g")
    set (GPUMODE on )
    message(${GPUMODE})    
    set(SYS_INC_DIR "/usr/local/cuda/include" "/usr/local/include/")
    set(SYS_LIB_DIR "/usr/local/bin" "usr/local/lib")
else ()
    set(CMAKE_CXX_FLAGS "-Wno-invalid-source-encoding -Wno-deprecated-register -Wno-writable-strings -Wno-parentheses -pthread -I_3rdparty_/ -Icommon/ -Icalib/ -Idaemon/ -Iendecode -Imanager/ -Istabil -O2 -g")
endif()

add_definitions(-D_DEBUG)
add_definitions(-DOPENCV) #for yolove_v2_class detection

set(PRJ_PATH        ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB PRJ_SRC
	${PRJ_PATH}/calib/*.cpp
	${PRJ_PATH}/common/*.cpp    
	${PRJ_PATH}/daemon/*.cpp
	${PRJ_PATH}/manager/*.cpp
    ${PRJ_PATH}/stabil/*.cpp
    ${PRJ_PATH}/main.cpp
)

list(REMOVE_ITEM PRJ_SRC ${PRJ_PATH}/stabil/Detection.cpp)

if( ${PLATFORM} STREQUAL "MACM1" OR ${PLATFORM} STREQUAL "LINUX")
    message("STREQUAL MACM1")
    list(REMOVE_ITEM PRJ_SRC ${PRJ_PATH}/manager/FrTrans.cpp)

endif()

file(GLOB PRJ_INC      ${PRJ_PATH}/calib/
                       ${PRJ_PATH}/common/
                       #${PRJ_PATH}/common
                       ${PRJ_PATH}/daemon/
                       ${PRJ_PATH}/manager
                       ${PRJ_PATH}/stabil
                       ${PRJ_PATH}/endecode/include
)

set(CURL_INC_DIR    ${PRJ_PATH}/_3rdparty_/curl/include)
set(DARKNET_PATH    ${PRJ_PATH}/_3rdparty_/darknet)
set(GSL_INC_DIR     ${PRJ_PATH}/_3rdparty_/gsl/include)
set(RJSON_INC_DIR   ${PRJ_PATH}/_3rdparty_/rapidjson/include)
set(SPD_INC_DIR     ${PRJ_PATH}/_3rdparty_/spdlog/include)  
set(ZL_INC_DIR      ${PRJ_PATH}/_3rdparty_/ZLToolKit/include)

if( ${PLATFORM} STREQUAL "MACM1" OR ${PLATFORM} STREQUAL "MACINTEL")
    set(DARKNET_LIB     ${DARKNET_PATH}/lib/${PLATFORM}/libdarknet.dylib)
    set(GSL_LIB_DIR     ${PRJ_PATH}/_3rdparty_/gsl/lib/${PLATFORM}/libgsl.dylib)
    set(SPD_LIB_DIR     ${PRJ_PATH}/_3rdparty_/spdlog/lib/${PLATFORM}/libspdlog.a)
    set(ZL_LIB_DIR      ${PRJ_PATH}/_3rdparty_/ZLToolKit/lib/${PLATFORM}/libZLToolKit.dylib)

elseif(${PLATFORM} STREQUAL "LINUX")
    set(DARKNET_LIB     ${DARKNET_PATH}/lib/${PLATFORM}/libdarknet.so)
    set(GSL_LIB_DIR     ${PRJ_PATH}/_3rdparty_/gsl/lib/${PLATFORM}/libgsl.so 
                        ${PRJ_PATH}/_3rdparty_/gsl/lib/${PLATFORM}/libgslcblas.so)
    set(SPD_LIB_DIR     ${PRJ_PATH}/_3rdparty_/spdlog/lib/${PLATFORM}/libspdlog.so)
    set(ZL_LIB_DIR      ${PRJ_PATH}/_3rdparty_/ZLToolKit/lib/${PLATFORM}/libZLToolKit.a)
endif()

message(${ZL_LIB_DIR})
message(${ZL_INC_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${PRJ_INC} ${CURL_INC_DIR} ${GSL_INC_DIR} ${OpenCV_INCLUDE_DIRS} ${RJSON_INC_DIR} ${SPD_INC_DIR} ${SYS_INC_DIR} ${ZL_INC_DIR} )
add_executable(zion ${PRJ_SRC})
target_link_libraries(zion ${GSL_LIB_DIR} ${OpenCV_LIBS} ${SPD_LIB_DIR} ${ZL_LIB_DIR})