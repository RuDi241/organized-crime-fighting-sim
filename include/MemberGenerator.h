#ifndef MEMBER_GENERATOR_H
#define MEMBER_GENERATOR_H

#include "Config.h"
enum class GangMemberType : int {
  GANG_MEMBER,
  SECRET_AGENT,
};

struct GangMemberMessage {
  long mtype; // Required for System V msg queues, typically > 0
  GangMemberType type;
  int ID;
  int rank;
  int trust;
};
class MemberGenerator {
public:
  MemberGenerator() = delete;
  MemberGenerator(const Config &config, int msq_id);

  void run();
  GangMemberMessage generate();

private:
  const Config &config;
  int msq_id = -1;
  int serial_id = 1;
};

#endif
