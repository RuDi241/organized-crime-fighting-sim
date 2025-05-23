#ifndef SECRET_AGENT_H
#define SECRET_AGENT_H

#include "GangMember.h"

class SecretAgent : public GangMember {
public:
  using GangMember::GangMember;
  SecretAgent() = delete;
};

#endif
