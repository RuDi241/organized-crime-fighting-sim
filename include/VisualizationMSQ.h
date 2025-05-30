#ifndef VISUALIZATION_MESSAGE_H
#define VISUALIZATION_MESSAGE_H

#include "MemberStruct.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

// Message type values (used as mtype)
enum MessageType : long {
  ADD_GANG = 1,
  REMOVE_GANG,
  UPDATE_GANG,
  ADD_MEMBER,
  REMOVE_MEMBER,
  UPDATE_MEMBER
};

struct VisualizationMessage {
  long mtype;
  int gangID;
  int memberIdx; // index of updated member in the gang
  int leaks;
  int phase;
  MemberStruct member;
};

class VisualizationMSQ {
public:
  static bool send(const VisualizationMessage &msg);
  static bool receive(VisualizationMessage &msg,
                      MessageType type = static_cast<MessageType>(0));
  static bool try_receive(VisualizationMessage &msg,
                          MessageType type = static_cast<MessageType>(0));

  // SET IN MAIN BEFORE USING THE CLASS
  static int msqid;
};

#endif
