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
#include "bufsize.h"
#include "prompt.h"

using namespace std;

int g_argc;
char **g_argv;

void GetArgs(char **servIP, in_port_t *servPort)
{
  char *port;

  if (g_argc < 2 || g_argc > 3)
  {
    DieWithUserMessage("Parameter(s)",
		       "<Server Address> <Echo Word> [<Server Port>]");
    exit(1);
  }

  *servIP = g_argv[1];
  *servPort = (g_argc == 3) ? atoi(g_argv[2]) : 7;
}

int main(int argc, char *argv[])
{
  char *servIP;
  in_port_t servPort;
  struct sockaddr_in servAddr;
  char msg[BUFSIZE];
  size_t msgLen;
  ssize_t numBytes;
  unsigned int totalBytesRcvd;
  int clientSock;


  g_argc = argc;
  g_argv = argv;

  GetArgs(&servIP, &servPort);

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

  do
  {
    fputs(prompt, stdout);
    fgets(msg, 256, stdin);

    msgLen = strlen(msg);
    numBytes = send(clientSock, msg, msgLen, 0);

    if (numBytes < 0)
      DieWithSystemMessage("send() failed");
    else if (numBytes != static_cast<size_t>(msgLen))
      DieWithUserMessage("send()", "sent unexpected number of bytes");
  } while (strcmp(msg, "quit\n") != 0);

  close(clientSock);
  return 0;
}
