cmake_minimum_required(VERSION 2.6)
project(Sockets)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_COMPILER g++) # needed for correct compilation (esp. debugging)
set(CMAKE_CC_COMPILER gcc)

# Echo Client sources
set(TCPEchoClientv4_SRC TCPEchoClientv4.cc)

# Echo Server sources
set(TCPEchoServerv4_SRC TCPEchoServerv4.cc)

# Msg Client sources
set(TCPMsgClient_SRC TCPMsgClient.cc)

# Msg Server sources
set(TCPMsgServer_SRC TCPMsgServer.cc)

# DieWithMessage library
set(DieWithMessage_SRC DieWithMessage.cc)
add_library(DieWithMessage ${DieWithMessage_SRC})

# HandleTCPClient library
set(HandleTCPClient_SRC HandleTCPClient.cc)
add_library(HandleTCPClient ${HandleTCPClient_SRC})

# HandleMsg library
set(HandleMsg_SRC HandleMsg.cc)
add_library(HandleMsg ${HandleMsg_SRC})

# Build echo client and link to libraries
add_executable(TCPEchoClientv4 ${TCPEchoClientv4_SRC})
target_link_libraries(TCPEchoClientv4 DieWithMessage)

# Build echo server and link to libraries
add_executable(TCPEchoServerv4 ${TCPEchoServerv4_SRC})
target_link_libraries(TCPEchoServerv4 DieWithMessage HandleTCPClient)

# Build msg client and link to libraries
add_executable(TCPMsgClient ${TCPMsgClient_SRC})
target_link_libraries(TCPMsgClient DieWithMessage)

# Build msg client and link to libraries
add_executable(TCPMsgServer ${TCPMsgServer_SRC})
target_link_libraries(TCPMsgServer DieWithMessage HandleMsg)
