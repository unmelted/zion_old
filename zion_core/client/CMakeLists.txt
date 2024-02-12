#Created by EunKyung Ma(ekma@livsmed.com) on 2024/02/12.

FILE(GLOB CLIENT_SRC
        *.cpp)

INCLUDE_DIRECTORIES(${PRJ_PATH}/common/include)
ADD_EXECUTABLE(ic_client ${CLIENT_SRC})
TARGET_LINK_LIBRARIES(ic_client  ics_common spdlog::spdlog fmt sqlite3)


