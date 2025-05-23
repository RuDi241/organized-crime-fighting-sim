#include "MemberGenerator.h"
#include "GangMember.h"
#include "MemberGeneratorMessage.h"
#include "NormalGangMember.h"
#include "SecretAgent.h"
#include <cstdlib>
#include <ctime>
#include <sys/msg.h>
#include <unistd.h>

MemberGenerator::MemberGenerator(const Config &config, int msq_id)
    : config(config), msq_id(msq_id) {
  srand(time(NULL));
}

GangMemberMessage MemberGenerator::generate() {
  float rnd = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

  return GangMemberMessage{
      1, // mtype (must be first and > 0)
      (rnd < config.agent.p_agent) ? GangMemberType::SECRET_AGENT
                                   : GangMemberType::GANG_MEMBER,
      serial_id++, config.gang.num_ranks, config.agent.initial_trust};
}

GangMember MemberGenerator::messageToMember(const GangMemberMessage &msg) {
  if (msg.type == GangMemberType::SECRET_AGENT) {
    SecretAgent member(msg);
    return member;
  } else {

    NormalGangMember member(msg);
    return member;
  }
}
void MemberGenerator::run() {
  while (true) {
    GangMemberMessage message = generate();

    // Send the message to the message queue
    if (msgsnd(msq_id, &message, sizeof(GangMemberMessage) - sizeof(long), 0) ==
        -1) {
      perror("msgsnd failed");
      // handle error as appropriate (break, continue, or exit)
    }

    sleep(config.gang.member_generation_delay);
  }
}
