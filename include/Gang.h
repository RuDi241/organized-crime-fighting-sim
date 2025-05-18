#ifndef GANG_H
#define GANG_H

#include <vector>
#include "Member.h"

class Gang {
public:
  std::vector<Member> members;
  Member &leader;
  // std::vector<Target> targets;
  void acceptMember(const Member &member);
  void startOperation();
  void leaveJail();

private:
};

#endif
