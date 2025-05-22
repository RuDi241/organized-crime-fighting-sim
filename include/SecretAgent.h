#ifndef SECRET_AGENT_H
#define SECRET_AGENT_H

#include "GangMember.h"

class SecretAgent : public GangMember {
  public:
    SecretAgent(int ID, int rank, int trust);
    SecretAgent() = delete;
};

#endif
