## 2022.03.23 
## created by Kelly (ekma@4dreplay.com)
##
cmake_minimum_required(VERSION 3.7)
project(zion)
set (PLATFORM MACM1) # available : MACM1, MACINTEL, WIN, ARM, LINUX
message(${PLATFORM})
set (GPUMODE OFF )

if( ${PLATFORM} STREQUAL "WIN" )
#    set(OpenCV_INCLUDE_DIRS ${PRJ_PATH}/3rd/opencv4.5.3/include/)
#    set(OpenCV_LIBS ${PRJ_PATH}/3rd/opencv4.5.3/lib/)
else()
    find_package( OpenCV REQUIRED )
endif()

set(CMAKE_CXX_STANDARD 17)
if( ${PLATFORM} STREQUAL "LINUX" )
    add_definitions(-D_LINUX_)    
set(CMAKE_CXX_FLAGS "-Wno-parentheses -pthread -I_3rdparty_/ -Icommon/ -Icalib/ -Idaemon/ -Iendecode -Imanager/ -Istabil -O2 -g")
    set(SYS_INC_DIR "/usr/local/cuda/include" "/usr/local/include/")
    set(SYS_LIB_DIR "/usr/local/bin" "usr/local/lib")
    message(${GPUMODE})    
    if( ${GPUMODE} STREQUAL "ON" )
        add_definitions(-DGPU)
    endif()
else ()
    add_definitions(-D_MAC_)    
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

if( ${PLATFORM} STREQUAL "MACM1" OR ${PLATFORM} STREQUAL "LINUX")
    message("STREQUAL MACM1 or LINUX - remove FrTrans")
    list(REMOVE_ITEM PRJ_SRC ${PRJ_PATH}/manager/FrTrans.cpp)

endif()

set(CURL_INC_DIR    ${PRJ_PATH}/_3rdparty_/curl/include)
set(DARKNET_INC_DIR    ${PRJ_PATH}/_3rdparty_/darknet/darknet.h ${PRJ_PATH}/_3rdparty_/darknet/yolo_v2_class.hpp )
set(GSL_INC_DIR     ${PRJ_PATH}/_3rdparty_/gsl/include)
set(RJSON_INC_DIR   ${PRJ_PATH}/_3rdparty_/rapidjson/include)
set(SPD_INC_DIR     ${PRJ_PATH}/_3rdparty_/spdlog/include)  
set(ZL_INC_DIR      ${PRJ_PATH}/_3rdparty_/ZLToolKit/include)


file(GLOB PRJ_INC      ${PRJ_PATH}/calib/
                       ${PRJ_PATH}/common/
                       ${PRJ_PATH}/daemon/
                       ${PRJ_PATH}/manager
                       ${PRJ_PATH}/stabil
                       ${PRJ_PATH}/endecode/include
)

if( ${PLATFORM} STREQUAL "MACM1" OR ${PLATFORM} STREQUAL "MACINTEL")
    set(DARKNET_LIB     ${PRJ_PATH}/_3rdparty_/darknet/lib/${PLATFORM}/libdarknet.dylib)
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

message(${DARKNET_INC_DIR})
message(${DARKNET_LIB})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
if( ${PLATFORM} STREQUAL "MACM1" OR ${PLATFORM} STREQUAL "MACINTEL")
    include_directories(${PRJ_INC} ${CURL_INC_DIR} ${GSL_INC_DIR} ${OpenCV_INCLUDE_DIRS} ${RJSON_INC_DIR} ${SPD_INC_DIR} ${SYS_INC_DIR} ${ZL_INC_DIR} ${DARKNET_INC_DIR})
    add_executable(zion ${PRJ_SRC})    
    target_link_libraries(zion ${GSL_LIB_DIR} ${OpenCV_LIBS} ${SPD_LIB_DIR} ${ZL_LIB_DIR} ${DARKNET_LIB})

elseif(${PLATFORM} STREQUAL "LINUX")
    include_directories(${PRJ_INC} ${CURL_INC_DIR} ${GSL_INC_DIR} ${OpenCV_INCLUDE_DIRS} ${RJSON_INC_DIR} ${SPD_INC_DIR} ${SYS_INC_DIR} ${ZL_INC_DIR} )
    add_executable(zion ${PRJ_SRC})        
    target_link_libraries(zion ${GSL_LIB_DIR} ${OpenCV_LIBS} ${SPD_LIB_DIR} ${ZL_LIB_DIR})
endif()

