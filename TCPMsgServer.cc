/* -*- Mode:C++; c-file-style:"bsd"; c-basic-offset:2; -*- */
/*
 * 1. Create a TCP socket using socket().
 * 2. Assign a port number to the socket with bind().
 * 3. Tell the system to allow connections to be made to that port, using
 *    listen().
 * 4. Repeatedly do the following:
 *     • Call accept() to get a new socket for each client connection.
 *     • Communicate with the client via that new socket using send() and
 *       recv().
 *     • Close the client connection using close().
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
#include "HandleMsg.h"

static const int MAXPENDING = 5;

int main(int argc, char *argv[])
{
  struct sockaddr_in clntAddr, servAddr;
  in_port_t servPort;

  if (argc != 2)
    DieWithUserMessage("Parameter(s)", "<Server Port>");

  servPort = atoi(argv[1]);

  int servSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
  if (servSock < 0)
    DieWithSystemMessage("socket() failed");

  memset(&servAddr, 0, sizeof(servAddr));
  servAddr.sin_family = AF_INET;
  servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servAddr.sin_port = htons(servPort);

  if (bind(servSock, (struct sockaddr*) &servAddr, sizeof(servAddr)) < 0)
    DieWithSystemMessage("bind() failed");

  if (listen(servSock, MAXPENDING) < 0)
    DieWithSystemMessage("listen() failed");

  while (true)
    {
      socklen_t clntAddrLen = sizeof(clntAddr);
      int clntSock = accept(servSock, (struct sockaddr *) &clntAddr,
			    &clntAddrLen);
      if (clntSock < 0)
	DieWithSystemMessage("accept() failed");

      char clntName[INET_ADDRSTRLEN];
      if (inet_ntop(AF_INET, &clntAddr.sin_addr.s_addr, clntName,
		    sizeof(clntName)) != NULL)

	printf("Handling client %s/%d\n", clntName, ntohs(clntAddr.sin_port));
      else
	puts("Unable to get client address");

      HandleMsg(clntSock);
    }
}
