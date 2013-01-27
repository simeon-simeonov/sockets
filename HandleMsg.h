/* -*- Mode:C++; c-file-style:"bsd"; c-basic-offset:2; -*- */

#ifndef HANDLEMSG_H
#define HANDLEMSG_H

#include <cstring>

#include <sys/socket.h>
#include <unistd.h>

void HandleMsg(int clntSocket);

#endif /* HANDLEMSG_H */
