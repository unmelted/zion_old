# # 2022.03.23
# # created by Kelly (ekma@4dreplay.com)
# #
cmake_minimum_required(VERSION 3.12)
project(zion)
set(PLATFORM LINUX) # available : APPLE LINUX
message(${PLATFORM})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-D_DEBUG)

set(PRJ_PATH ${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB PRJ_SRC
    common/*.cpp
    daemon/*.cpp
    manager/*.cpp
    main.cpp
)
file(GLOB PRJ_INC manager common daemon)

find_package(spdlog REQUIRED)

set(CURL_INC_DIR ${PRJ_PATH}/_3rdparty_/curl/include)
set(RJSON_INC_DIR ${PRJ_PATH}/_3rdparty_/rapidjson/include)

INCLUDE_DIRECTORIES(/opt/homebrew/include /usr/local/include ${PRJ_INC} ${RJSON_INC_DIR})
LINK_DIRECTORIES(/opt/homebrew/lib /usr/local/lib)

add_executable(zion ${PRJ_SRC})
target_link_libraries(zion spdlog::spdlog fmt)
