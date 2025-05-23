#ifndef AGENT_TO_POLICE_MESSAGE_H
#define AGENT_TO_POLICE_MESSAGE_H

enum class AgentMessageType : int {
    NORMAL_INFO,
    ATTACK,
};

struct AgentMessage {
  long mtype; // Required for System V msg queues, typically > 0
  AgentMessageType type;
  int MessageID;
  int gangID;
  int weight;
};
#endif