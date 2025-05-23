#ifndef MEMBER_GENERATOR_H
#define MEMBER_GENERATOR_H

#include "Config.h"
#include "MemberGeneratorMessage.h"
#include "GangMember.h"

class MemberGenerator {
public:
  MemberGenerator() = delete;
  MemberGenerator(const Config &config, int msq_id);

  static GangMember messageToMember(const GangMemberMessage &msg);
  void run();
  GangMemberMessage generate();

private:
  const Config &config;
  int msq_id;
  int serial_id = 1;
};

#endif
