#ifndef VISUALIZATION_MESSAGE_H
#define VISUALIZATION_MESSAGE_H

#include "GangStruct.h"
#include "MemberStruct.h"

#include <cstdio>
#include <cstring>
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
  MemberStruct member;
};

class MessageQueue {
public:
  explicit MessageQueue(int queue_id);
  ~MessageQueue();

  bool send(const VisualizationMessage &msg) const;
  bool receive(VisualizationMessage &msg,
               MessageType type = static_cast<MessageType>(0)) const;
  bool try_receive(VisualizationMessage &msg,
                   MessageType type = static_cast<MessageType>(0)) const;

private:
  int msgid;
};

#endif
