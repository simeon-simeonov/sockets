/* -*- Mode:C++; c-file-style:"bsd"; c-basic-offset:2; -*- */

#include "HandleMsg.h"
#include "DieWithMessage.h"
#include "bufsize.h"

using namespace std;

void HandleMsg(int clntSocket) {
  char buffer[BUFSIZE];
  char msg[BUFSIZE];
  ssize_t numBytesRcvdOld, numBytesRcvd, numBytesSent;
  char *ptr;
  int i;

  numBytesRcvdOld = 0;

  while((numBytesRcvd = recv(clntSocket, buffer, BUFSIZE - 1, 0)) > 0
	&& (numBytesRcvd != numBytesRcvdOld))
  {
    strncpy(msg, buffer, BUFSIZE - 1);
    msg[BUFSIZE - 1] = '\0';

    if (numBytesRcvd < 0)
      DieWithSystemMessage("recv() failed");

    ptr = strchr(msg, '\n');
    i = (ptr - msg) < BUFSIZE - 1? ptr - msg : BUFSIZE - 2;
    msg[i + 1] = '\0';
    printf("%s", msg);
    buffer[0] = '\0';
    msg[0] = '\0';
  }

  close(clntSocket);
  puts("***Client Disconnected***");
}
