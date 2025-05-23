#ifndef NORMAL_GANG_MEMBER_H
#define NORMAL_GANG_MEMBER_H

#include "GangMember.h"

class NormalGangMember : public GangMember {
public:
  using GangMember::GangMember;
  NormalGangMember() = delete;
};

#endif
