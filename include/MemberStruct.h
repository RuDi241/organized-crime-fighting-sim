#ifndef MEMBER_STRUCT_H
#define MEMBER_STRUCT_H

#include "MemberGeneratorMessage.h"

struct MemberStruct {
  int ID;
  int rank;
  int trust;
  int preparation_counter;
  bool ready;
  GangMemberType type;
};

#endif
