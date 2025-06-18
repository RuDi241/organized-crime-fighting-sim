#include "MemberGenerator.h"
#include "GangMember.h"
#include "MemberGeneratorMessage.h"
#include "NormalGangMember.h"
#include "SecretAgent.h"
#include <cstdlib>
#include <ctime>
#include <sys/msg.h>
#include <unistd.h>
#include <iostream>
#include <memory>
MemberGenerator::MemberGenerator(const Config &config, int msq_id,
                                 int member_to_police_msq_id)
    : config(config), msqID(msq_id),
      memberToPoliceMsqID(member_to_police_msq_id) {
  srand(time(NULL));
}

GangMemberMessage MemberGenerator::generate() {
  float rnd = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  return GangMemberMessage{
      1, // mtype (must be first and > 0)
      (rnd < config.agent.p_agent) ? GangMemberType::SECRET_AGENT
                                   : GangMemberType::GANG_MEMBER,
      memberToPoliceMsqID, // ID of the message queue for police communication
      serialID++,
      rand() % 2 + 1, // Random rank between 1 and 2
      rand() % config.agent.initial_trust + config.agent.initial_trust/2}; // random trust around initial trust
}

std::unique_ptr<GangMember>
MemberGenerator::messageToMember(const GangMemberMessage &msg,
                                 const int gangID) {
  if (msg.type == GangMemberType::SECRET_AGENT) {
    return std::make_unique<SecretAgent>(msg, msg.memberToPoliceMsqID, gangID);
  } else {
    return std::make_unique<NormalGangMember>(msg, gangID);
  }
}
void MemberGenerator::run() {
  while (true) {
    GangMemberMessage message = generate();

    // Send the message to the message queue
    if (msgsnd(msqID, &message, sizeof(GangMemberMessage) - sizeof(long), 0) ==
        -1) {
      // perror("msgsnd failed");
      //  handle error as appropriate (break, continue, or exit)
    }

    sleep(config.gang.member_generation_delay);
  }
}
