#include "VisualizationMSQ.h"
#include <cstring>
#include <cerrno>
#include <cstdio>

int VisualizationMSQ::msqid = -1;

bool VisualizationMSQ::send(const VisualizationMessage &msg) {
  size_t size = sizeof(VisualizationMessage) - sizeof(long);
  if (msgsnd(msqid, &msg, size, 0) == -1) {
    perror("msgsnd failed");
    return false;
  }
  return true;
}

bool VisualizationMSQ::receive(VisualizationMessage &msg, MessageType type) {
  size_t size = sizeof(VisualizationMessage) - sizeof(long);
  if (msgrcv(msqid, &msg, size, type, 0) == -1) {
    perror("msgrcv failed");
    return false;
  }
  return true;
}

bool VisualizationMSQ::try_receive(VisualizationMessage &msg,
                                   MessageType type) {
  size_t size = sizeof(VisualizationMessage) - sizeof(long);
  if (msgrcv(msqid, &msg, size, type, IPC_NOWAIT) == -1) {
    if (errno != ENOMSG) {
      perror("msgrcv failed");
    }
    return false;
  }
  return true;
}
