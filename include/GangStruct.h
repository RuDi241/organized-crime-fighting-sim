#ifndef GANG_STRUCT_H
#define GANG_STRUCT_H

#include "MemberStruct.h"
#include <vector>

struct GangStruct {
  std::vector<MemberStruct> GangMembers;
  int ID;
  int capacity;
};

#endif
