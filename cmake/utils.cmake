cmake_minimum_required(VERSION 3.28)
project(1)

set(CMAKE_CXX_STANDARD 17)

add_executable(1 main.cpp Transaction.cpp Transaction.h Blockchain.h Blockchain.cpp Utilities.h Utilities.cpp)