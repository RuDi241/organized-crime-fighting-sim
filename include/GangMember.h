#ifndef GANG_MEMBER_H
#define GANG_MEMBER_H

#include <string>
#include "Member.h"

class GangMember : public Member {
public:
  GangMember(const std::string name, int ID, int rank, int trust);
  GangMember() = delete;
  virtual ~GangMember();
};
#endif
