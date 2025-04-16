cmake_minimum_required(VERSION 3.30)
project(CourseDesign LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(FLTK_DIR "F:/fltk-1.4.2/cmake-build-release")

find_package(FLTK REQUIRED)

file(GLOB SRCS "src/*.cpp")

include_directories(
        "include"
        "include/Protocols"
        "F:/WpdPack/Include"
        ${FLTK_INCLUDE_DIRS}
)

link_directories("F:/WpdPack/Lib/x64")

add_executable(WinCapLiteSniffer ${SRCS})

target_link_libraries(WinCapLiteSniffer PRIVATE wpcap Ws2_32 ${FLTK_LIBRARIES})