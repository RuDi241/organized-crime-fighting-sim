#ifndef MEMBER_GENERATOR_MESSAGE_H
#define MEMBER_GENERATOR_MESSAGE_H

enum GangMemberType {
  GANG_MEMBER,
  SECRET_AGENT,
};

struct GangMemberMessage {
  long mtype; // Required for System V msg queues, typically > 0
  GangMemberType type;
  int memberToPoliceMsqID; // ID of the message queue for police communication
  int ID;
  int rank;
  int trust;
};
#endif
