#include "VisualizationMSQ.h"
#include <cstring>
#include <cerrno>
#include <cstdio>
#include <stdexcept>

MessageQueue::MessageQueue(int queue_id) : msgid(queue_id) {
  if (msgid == -1) {
    throw std::runtime_error("Invalid message queue ID");
  }
}

MessageQueue::~MessageQueue() {
  // Does not destroy the message queue; ownership is external
}

bool MessageQueue::send(const VisualizationMessage &msg) const {
  size_t size = sizeof(VisualizationMessage) - sizeof(long);
  if (msgsnd(msgid, &msg, size, 0) == -1) {
    perror("msgsnd failed");
    return false;
  }
  return true;
}

bool MessageQueue::receive(VisualizationMessage &msg, MessageType type) const {
  size_t size = sizeof(VisualizationMessage) - sizeof(long);
  if (msgrcv(msgid, &msg, size, type, 0) == -1) {
    perror("msgrcv failed");
    return false;
  }
  return true;
}

bool MessageQueue::try_receive(VisualizationMessage &msg,
                               MessageType type) const {
  size_t size = sizeof(VisualizationMessage) - sizeof(long);
  if (msgrcv(msgid, &msg, size, type, IPC_NOWAIT) == -1) {
    if (errno != ENOMSG) {
      perror("msgrcv failed");
    }
    return false;
  }
  return true;
}
