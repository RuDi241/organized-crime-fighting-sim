#ifndef MEMBER_STRUCT_H
#define MEMBER_STRUCT_H

#include "MemberGeneratorMessage.h"

struct MemberStruct {
  int ID;
  int rank;
  int trust;
  int preparation_counter = 0;
  bool ready = false;
  bool thread_running = false;
  bool should_stop = false;
  GangMemberType type;
};

#endif
