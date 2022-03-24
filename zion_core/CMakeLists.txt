## 2022.03.23 
## created by Kelly (ekma@4dreplay.com)
##
cmake_minimum_required(VERSION 3.7)
project(zion)
set (PLATFORM MACM1) # available : MACM1, MACINTEL, WIN, ARM, LINUX
set (GPUMODE off )
message(${GPUMODE})
message(${PLATFORM})

if( ${PLATFORM} STREQUAL "WIN" )
#    set(OpenCV_INCLUDE_DIRS ${PRJ_PATH}/3rd/opencv4.5.3/include/)
#    set(OpenCV_LIBS ${PRJ_PATH}/3rd/opencv4.5.3/lib/)
else()
    find_package( OpenCV REQUIRED )
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wno-invalid-source-encoding -Wno-deprecated-register -Wno-writable-strings -Wno-parentheses -I_3rdparty_/ -Icommon/ -Icalib/ -Idaemon/ -Iendecode -Imanager/ -Istabil -O2 -g")

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

if( ${PLATFORM} STREQUAL "MACM1" )
    message("STREQUAL MACM1")
    list(REMOVE_ITEM PRJ_SRC ${PRJ_PATH}/manager/FrTrans.cpp)
endif()

file(GLOB PRJ_INC      ${PRJ_PATH}/calib/*.hpp
                       ${PRJ_PATH}/common/*.hpp
                       ${PRJ_PATH}/common/*.h
                       ${PRJ_PATH}/daemon/*.h
                       ${PRJ_PATH}/manager/*.hpp
                       ${PRJ_PATH}/stabil/*.hpp
)


set(DARKNET_PATH    ${PRJ_PATH}/_3rdparty_/darknet)
set(DARKNET_LIB     ${DARKNET_PATH}/lib/${PLATFORM}/libdarknet.dylib)
set(GSL_INC_DIR     ${PRJ_PATH}/_3rdparty_/gsl/include)
set(GSL_LIB_DIR     ${PRJ_PATH}/_3rdparty_/gsl/lib/${PLATFORM}/libgsl.dylib)
set(RJSON_INC_DIR   ${PRJ_PATH}/_3rdparty_/rapidjson/include)
set(SPD_INC_DIR     ${PRJ_PATH}/_3rdparty_/spdlog/include)  
set(SPD_LIB_DIR     ${PRJ_PATH}/_3rdparty_/spdlog/lib/${PLATFORM}/libspdlog.a)
set(ZL_INC_DIR      ${PRJ_PATH}/_3rdparty_/ZLToolKit/include)
set(ZL_LIB_DIR      ${PRJ_PATH}/_3rdparty_/ZLToolKit/lib/${PLATFORM}/libZLToolKit.dylib)

message(${ZL_LIB_DIR})
message(${ZL_INC_DIR})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${PRJ_INC} ${GSL_INC_DIR} ${OpenCV_INCLUDE_DIRS} ${RJSON_INC_DIR} ${SPD_INC_DIR} ${ZL_INC_DIR} )
add_executable(zion ${PRJ_SRC})
target_link_libraries(zion ${GSL_LIB_DIR} ${OpenCV_LIBS} ${SPD_LIB_DIR} ${ZL_LIB_DIR})