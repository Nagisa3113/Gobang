cmake_minimum_required(VERSION 3.17)
project(Chess)

set(CMAKE_CXX_STANDARD 17)

add_executable(Chess main.cpp Gobang.h MCTS.cpp AlphaBeta.cpp)