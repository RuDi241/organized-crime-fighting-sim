#ifndef GANG_STRUCT_H
#define GANG_STRUCT_H

#include "MemberStruct.h"
#include <vector>

enum class GangPhase { PREPARATION, OPERATION, JAILED };
struct GangStruct {
  std::vector<MemberStruct> GangMembers;
  int ID;
  int capacity;
  int leaks;
  GangPhase phase;
};

#endif
