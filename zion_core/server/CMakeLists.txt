#Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/12.

FILE(GLOB SERVER_SRC
        *.cpp)

INCLUDE_DIRECTORIES(${PRJ_PATH}/common/include)
ADD_EXECUTABLE(ic_server ${SERVER_SRC}
        server_task.h
        server_task.cpp)

IF(APPLE)
    TARGET_LINK_LIBRARIES(ic_server ics_common spdlog::spdlog fmt sqlite3)
ELSE()
    TARGET_LINK_LIBRARIES(ic_server ics_common spdlog::spdlog sqlite3)
ENDIF()



