#ifndef INFORMATION_MESSAGE_H
#define INFORMATION_MESSAGE_H

enum class InformationMessageType : int {
    NORMAL_INFO,
    ATTACK,
};

struct InformationMessage {
  long mtype; // Required for System V msg queues, typically > 0
  InformationMessageType type;
  int MessageID;
  int gangID;
  int weight;
};
#endif