/* -*- Mode:C++; c-file-style:"bsd"; c-basic-offset:2; -*- */

/*
 * This program does the following (p. 19):
 * 1. Create a TCP socket using socket().
 * 2. Establish a connection to the server using connect().
 * 3. Communicate using send and recv().
 * 4. Close the connection with close().
 *
 * API:
 * socket() - creates a socket
 * connect() - connect client socket to socket specified in the other param
 * send()/recv() - send/receive data (the echo string)
 * close() - inform remote socket of end of connection and free resources
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "DieWithMessage.h"

#define BUFSIZE 256

int g_argc;
char **g_argv;

void GetArgs(char **servIP, char **echoString, in_port_t *servPort)
{
  char *port;

  if (g_argc < 3 || g_argc > 4)
  {
    DieWithUserMessage("Parameter(s)",
		       "<Server Address> <Echo Word> [<Server Port>]");
    exit(1);
  }

  *servIP = g_argv[1];
  *echoString = g_argv[2];
  *servPort = (g_argc == 4) ? atoi(g_argv[3]) : 7;
}

int main(int argc, char *argv[])
{
  char *servIP;
  char *echoString;
  in_port_t servPort;
  struct sockaddr_in servAddr;
  char buffer[BUFSIZE];
  size_t echoStringLen;
  ssize_t numBytes;
  unsigned int totalBytesRcvd;
  int clientSock;

  g_argc = argc;
  g_argv = argv;

  GetArgs(&servIP, &echoString, &servPort);

  clientSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (clientSock < 0)
    DieWithSystemMessage("socket() failed");

  memset(&servAddr, 0, sizeof(servAddr));

  servAddr.sin_family = AF_INET;
  servAddr.sin_port = htons(servPort);
  int retVal = inet_pton(AF_INET, servIP, &servAddr.sin_addr.s_addr);

  if (retVal == 0)
    DieWithUserMessage("inet_pton() failed", "invalid address string");
  else if (retVal < 0)
    DieWithSystemMessage("inet_pton() failed");

  if (connect(clientSock, (struct sockaddr *) &servAddr, sizeof(servAddr))
      < 0)
    DieWithSystemMessage("connect() failed");

  echoStringLen = strlen(echoString);
  numBytes = send(clientSock, echoString, echoStringLen, 0);

  if (numBytes < 0)
    DieWithSystemMessage("send() failed");
  else if (numBytes != static_cast<size_t>(echoStringLen))
    DieWithUserMessage("send()", "sent unexpected number of bytes");

  totalBytesRcvd = 0;
  fputs("Received: ", stdout);

  while (totalBytesRcvd < echoStringLen) {
    numBytes = recv(clientSock, buffer, BUFSIZE - 1, 0);

    if (numBytes < 0)
      DieWithSystemMessage("recv() failed");
    else if (numBytes == 0)
      DieWithUserMessage("recv()", "connection closed prematurely");

    totalBytesRcvd += numBytes;
    buffer[numBytes] = '\0';

    fputs(buffer, stdout);
  }

  fputc('\n', stdout);
  close(clientSock);
  return 0;
}
