#Created by EunKyung Ma(ekma@livsmed.com) on 2024/01/05.

CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT(ic)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_INCLUDE_CURRENT_DIR ON)

FIND_PACKAGE(spdlog REQUIRED)

ADD_DEFINITIONS(-D_DEBUG)

SET(PRJ_PATH ${CMAKE_CURRENT_SOURCE_DIR})

FILE(GLOB COMMON_INC ${PRJ_PATH}/common/include)
SET(CURL_INC_DIR ${PRJ_PATH}/_3rdparty_/curl/include)
SET(RJSON_INC_DIR ${PRJ_PATH}/_3rdparty_/rapidjson/include)

SET(ics_common_src
        ${PRJ_PATH}/common/src/db_manager.cpp
        ${PRJ_PATH}/common/src/error_manager.cpp
        ${PRJ_PATH}/common/src/event_manager.cpp
        ${PRJ_PATH}/common/src/ic_util.cpp
        ${PRJ_PATH}/common/src/logger.cpp
        ${PRJ_PATH}/common/src/message_sender.cpp
        ${PRJ_PATH}/common/src/socket_message.cpp
        ${PRJ_PATH}/common/src/task_manager.cpp)

ADD_LIBRARY(ics_common ${ics_common_src})

INCLUDE_DIRECTORIES(
        /opt/homebrew/include
        /usr/local/include
        ${COMMON_INC}
        ${RJSON_INC_DIR})

LINK_DIRECTORIES(
        /opt/homebrew/lib
        /usr/local/lib
        /opt/homebrew/opt/zlib/lib)

ADD_SUBDIRECTORY(client)
ADD_SUBDIRECTORY(server)
