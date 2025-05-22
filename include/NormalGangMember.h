#ifndef NORMAL_GANG_MEMBER_H
#define NORMAL_GANG_MEMBER_H

#include "GangMember.h"

class NormalGangMember : public GangMember {
  public:
    NormalGangMember(int ID, int rank, int trust);
    NormalGangMember() = delete;
};

#endif
