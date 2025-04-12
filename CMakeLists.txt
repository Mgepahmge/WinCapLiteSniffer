cmake_minimum_required(VERSION 3.30)
project(CourseDesign LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

file(GLOB SRCS "src/*.cpp")

include_directories(
        "include"
        "F:/WpdPack/Include"
)

link_directories("F:/WpdPack/Lib/x64")

add_executable(WinCapLiteSniffer ${SRCS})

target_link_libraries(WinCapLiteSniffer PRIVATE wpcap Ws2_32)

