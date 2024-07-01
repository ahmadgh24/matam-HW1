cmake_minimum_required(VERSION 3.28)
project(1)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR})


add_library(
        1
        BlockChain.h
        Transaction.h
        Utilities.h
        Transaction.cpp
        BlockChain.cpp 
        Utilities.cpp
        main.cpp
)

target_include_directories(
        mtm_blockchain_lib
        PUBLIC
        .
)


add_executable(
        mtm_blockchain
)

target_link_libraries(mtm_blockchain)