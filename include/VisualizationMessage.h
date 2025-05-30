#ifndef VISUALIZATION_MESSAGE_H
#define VISUALIZATION_MESSAGE_H

#include "GangStruct.h"
#include "MemberStruct.h"

struct VisualizationMessage {
  enum class MessageType {
    ADD_GANG,
    REMOVE_GANG,
    UPDATE_GANG,
    ADD_MEMBER,
    REMOVE_MEMBER,
    UPDATE_MEMBER
  } type;

  int gangID = -1;   // Needed for all except ADD_GANG
  int memberID = -1; // Needed only for member-related messages

  // Optional payloads depending on message type
  GangStruct gangData;
  MemberStruct memberData;
};

#endif
