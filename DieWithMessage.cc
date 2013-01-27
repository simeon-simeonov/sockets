/* -*- Mode:C++; c-file-style:"bsd"; c-basic-offset:2; -*- */

#include "DieWithMessage.h"

void DieWithUserMessage(const char *msg, const char *detail) {
  fputs(msg, stderr);
  fputs(": ", stderr);
  fputs(detail, stderr);
  fputc('\n', stderr);
  exit(1);
}
void DieWithSystemMessage(const char *msg) {
  perror(msg);
  exit(1);
}
