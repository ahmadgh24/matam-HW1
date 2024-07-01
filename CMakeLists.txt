cmake_minimum_required(VERSION 3.28)
project(1)

set(CMAKE_CXX_STANDARD 17)

add_library(
        BlockChain.h
        Transaction.h
        Utilities.h
        Transaction.cpp Blockchain.cpp Utilities.cpp
)



add_executable(
        mtm_blockchain
        main.cpp
)

