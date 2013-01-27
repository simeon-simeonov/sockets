/* -*- Mode:C++; c-file-style:"bsd"; c-basic-offset:2; -*- */

#ifndef HANDLETCPCLIENT_H
#define HANDLETCPCLIENT_H

#include <cstring>

#include <sys/socket.h>
#include <unistd.h>

void HandleTCPClient(int clntSocket);

#endif /* HANDLETCPCLIENT_H */
