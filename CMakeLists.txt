cmake_minimum_required(VERSION 2.6)
project(Sockets)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER g++) # needed for correct compilation (esp. debugging)
set(CMAKE_CC_COMPILER gcc)

# Client sources
set(TCPEchoClientv4_SRC TCPEchoClientv4.cc)

# Server sources
set(TCPEchoServerv4_SRC TCPEchoServerv4.cc)

# DieWithMessage library
set(DieWithMessage_SRC DieWithMessage.cc)
add_library(DieWithMessage ${DieWithMessage_SRC})

# HandleTCPClient library
set(HandleTCPClient_SRC HandleTCPClient.cc)
add_library(HandleTCPClient ${HandleTCPClient_SRC})

# Build client and link to libraries
add_executable(TCPEchoClientv4 ${TCPEchoClientv4_SRC})
target_link_libraries(TCPEchoClientv4 DieWithMessage)

# Build server and link to libraries
add_executable(TCPEchoServerv4 ${TCPEchoServerv4_SRC})
target_link_libraries(TCPEchoServerv4 DieWithMessage HandleTCPClient)