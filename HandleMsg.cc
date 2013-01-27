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

  do {
    numBytesRcvdOld = 0;
    numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
    strncpy(msg, buffer, BUFSIZE - 1);
    msg[BUFSIZE] = '\0';

    if (numBytesRcvd < 0)
      DieWithSystemMessage("recv() failed");

    ptr = strchr(msg, '\n');
    i = (ptr - msg) < BUFSIZE ? ptr - msg : BUFSIZE - 1;
    msg[i + 1] = '\0';
    printf("%s", msg);

    while (numBytesRcvd > numBytesRcvdOld)
    {
      numBytesRcvdOld = numBytesRcvd;
      numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
      strncpy(msg, buffer, BUFSIZE - 1);
      msg[BUFSIZE] = '\0';

      if (numBytesRcvd < 0)
	DieWithSystemMessage("recv() failed");

      ptr = strchr(msg, '\n');
      i = (ptr - msg) < BUFSIZE ? ptr - msg : BUFSIZE - 1;
      msg[i + 1] = '\0';
      printf("%s", msg);
    }

    numBytesRcvdOld = 0;
  } while (numBytesRcvd > 0);

  close(clntSocket);
}
