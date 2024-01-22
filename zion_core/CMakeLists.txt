#Created by EunKyung Ma(ekma@livsmed.com) on 2024/01/05.

cmake_minimum_required(VERSION 3.12)
project(zion)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(spdlog REQUIRED)

add_definitions(-D_DEBUG)

set(PRJ_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB PRJ_SRC
    src/*.cpp
    main.cpp
)

file(GLOB PRJ_INC include)
set(CURL_INC_DIR ${PRJ_PATH}/_3rdparty_/curl/include)
set(RJSON_INC_DIR ${PRJ_PATH}/_3rdparty_/rapidjson/include)

INCLUDE_DIRECTORIES(
        /opt/homebrew/include
        /usr/local/include
        ${PRJ_INC}
        ${RJSON_INC_DIR})

LINK_DIRECTORIES(
        /opt/homebrew/lib
        /usr/local/lib)

add_executable(zion ${PRJ_SRC})
target_link_libraries(zion spdlog::spdlog fmt sqlite3)
