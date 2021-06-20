cmake_minimum_required(VERSION 3.16)
project(code)

set(CMAKE_CXX_STANDARD 14)

add_executable(code cmake-build-debug/TrainRelated.cpp cmake-build-debug/OrderRelated.cpp cmake-build-debug/Connect.cpp cmake-build-debug/Connect.hpp cmake-build-debug/OrderRelated.hpp cmake-build-debug/BasicElements.hpp cmake-build-debug/TrainRelated.hpp cmake-build-debug/Include.hpp cmake-build-debug/UserRelated.hpp cmake-build-debug/UserRelated.cpp cmake-build-debug/main.cpp cmake-build-debug/File_oi.hpp)