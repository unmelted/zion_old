# # 2022.03.23
# # created by Kelly (ekma@4dreplay.com)
# #
cmake_minimum_required(VERSION 3.12)
project(zion)
set(PLATFORM LINUX) # available : MACM1, MACINTEL, WIN, ARM, LINUX
message(${PLATFORM})
set(GPUMODE OFF)

if(${PLATFORM} STREQUAL "WIN")
# set(OpenCV_INCLUDE_DIRS ${PRJ_PATH}/3rd/opencv4.5.3/include/)
# set(OpenCV_LIBS ${PRJ_PATH}/3rd/opencv4.5.3/lib/)
else()
    find_package(OpenCV REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 17)

if(PLATFORM STREQUAL "LINUX")
    message(STATUS "linux platform build1.")
    add_definitions(-D_LINUX_)
    set(CMAKE_CXX_FLAGS "-Wno-parentheses -pthread -I_3rdparty_/ -Icommon/ -Idaemon/-Imanager/ -O2 -g")
    set(SYS_INC_DIR "/usr/local/cuda/include" "/usr/local/include/")
    set(SYS_LIB_DIR "/usr/local/bin" "usr/local/lib")
    message(${GPUMODE})

    if(${GPUMODE} STREQUAL "ON")
        add_definitions(-DGPU)
    endif()
else()
    add_definitions(-D_MAC_)
    set(CMAKE_CXX_FLAGS "-Wno-invalid-source-encoding -Wno-deprecated-register -Wno-writable-strings -Wno-parentheses -pthread -I_3rdparty_/ -Icommon/ -Idaemon/ -Imanager/ -O2 -g")
endif()

add_definitions(-D_DEBUG)
add_definitions(-DOPENCV) # for yolove_v2_class detection

set(PRJ_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB PRJ_SRC
    common/*.cpp
    daemon/*.cpp
    manager/*.cpp
    main.cpp
)

set(CURL_INC_DIR ${PRJ_PATH}/_3rdparty_/curl/include)
set(RJSON_INC_DIR ${PRJ_PATH}/_3rdparty_/rapidjson/include)
set(SPD_INC_DIR ${PRJ_PATH}/_3rdparty_/spdlog/include)
set(ZL_INC_DIR ${PRJ_PATH}/_3rdparty_/ZLToolKit/include)

file(GLOB PRJ_INC manager common daemon)

message(${PRJ_INC})
message(${PRJ_PATH})

if(${PLATFORM} STREQUAL "MACM1" OR ${PLATFORM} STREQUAL "MACINTEL")
    set(SPD_LIB_DIR ${PRJ_PATH}/_3rdparty_/spdlog/lib/${PLATFORM}/libspdlog.a)
    set(ZL_LIB_DIR ${PRJ_PATH}/_3rdparty_/ZLToolKit/lib/${PLATFORM}/libZLToolKit.dylib)

elseif(PLATFORM STREQUAL "LINUX")
    set(SPD_LIB_DIR ${PRJ_PATH}/_3rdparty_/spdlog/lib/${PLATFORM}/libspdlog.so)
    set(ZL_LIB_DIR ${PRJ_PATH}/_3rdparty_/ZLToolKit/lib/${PLATFORM}/libZLToolKit.a)
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

if(${PLATFORM} STREQUAL "MACM1" OR ${PLATFORM} STREQUAL "MACINTEL")
    include_directories(${PRJ_INC} ${CURL_INC_DIR} ${OpenCV_INCLUDE_DIRS} ${RJSON_INC_DIR} ${SPD_INC_DIR} ${SYS_INC_DIR} ${ZL_INC_DIR})
    add_executable(zion ${PRJ_SRC})
    target_link_libraries(zion ${GSL_LIB_DIR} ${OpenCV_LIBS} ${SPD_LIB_DIR} ${ZL_LIB_DIR})

elseif(PLATFORM STREQUAL "LINUX")
    include_directories(${PRJ_INC} ${CURL_INC_DIR} ${OpenCV_INCLUDE_DIRS} ${RJSON_INC_DIR} ${SPD_INC_DIR} ${SYS_INC_DIR} ${ZL_INC_DIR})
    add_executable(zion ${PRJ_SRC})
    target_link_libraries(zion ${OpenCV_LIBS} ${SPD_LIB_DIR} ${ZL_LIB_DIR})
endif()
