#ifndef MEMBER_GENERATOR_H
#define MEMBER_GENERATOR_H

#include "Config.h"
#include "MemberGeneratorMessage.h"
#include "GangMember.h"
#include <memory>
class MemberGenerator {
public:
  MemberGenerator() = delete;
  MemberGenerator(const Config &config, int msq_id,
                  int member_to_police_msq_id);

  static std::unique_ptr<GangMember>
  messageToMember(const GangMemberMessage &msg, const int gangID);
  void run();
  GangMemberMessage generate();

private:
  const Config &config;
  int msqID;
  int memberToPoliceMsqID;
  int serialID = 1;
};

#endif
