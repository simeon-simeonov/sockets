/* -*- Mode:C++; c-file-style:"bsd"; c-basic-offset:2; -*- */

#include "HandleTCPClient.h"
#include "DieWithMessage.h"
#include "bufsize.h"


void HandleTCPClient(int clntSocket) {
  char buffer[BUFSIZE];
  ssize_t numBytesRcvdOld =0;
  ssize_t numBytesRcvd, numBytesSent;

  numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);
  if (numBytesRcvd < 0)
    DieWithSystemMessage("recv() failed");

  while (numBytesRcvd > 0) {
    numBytesSent = send(clntSocket, buffer, numBytesRcvd, 0);

    if (numBytesSent < 0)
      DieWithSystemMessage("send() failed");
    else if (numBytesSent != numBytesRcvd)
      DieWithUserMessage("send()", "sent unexpected number of bytes");

    while (numBytesRcvd != numBytesRcvdOld)
    {
      numBytesRcvdOld = numBytesRcvd;
      numBytesRcvd = recv(clntSocket, buffer, BUFSIZE, 0);

      if (numBytesRcvd < 0)
	DieWithSystemMessage("recv() failed");

    }
    numBytesRcvdOld = 0;
  }
  close(clntSocket);
}
