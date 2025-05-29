#ifndef SECRET_AGENT_H
#define SECRET_AGENT_H

#include "GangMember.h"
#include "AgentToPoliceMessage.h"

class SecretAgent : public GangMember {
public:
  SecretAgent(int ID, int rank, int trust, int policeMsqID);
  SecretAgent(const GangMemberMessage &msg, int policeMsqID);
  SecretAgent() = delete;
  
  void receiveInformation(InformationMessage message) override;

private:
  int policeMsqID; // Message queue ID for communicating with police
  void sendToPolice(InformationMessage message);
};

#endif
