cmake_minimum_required(VERSION 3.10)
project(tcp_test)

# C++ 표준 설정
set(CMAKE_CXX_STANDARD 14)

# 소스 파일 추가
set(SOURCE_FILES test_main.cpp)

# 실행 파일 추가
add_executable(tcp_test ${SOURCE_FILES})

# 필요한 경우 추가 라이브러리 링크
target_link_libraries(tcp_test pthread)
