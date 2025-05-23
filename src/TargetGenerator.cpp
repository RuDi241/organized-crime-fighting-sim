#include "TargetGenerator.h"
#include "Target.h"
#include <sys/msg.h>
#include <unistd.h>

TargetGenerator::TargetGenerator(const Config &config, int msq_id)
    : config(config), msq_id(msq_id) {}

Target TargetGenerator::messageToTarget(const TargetMessage &message) {
  return message.target;
}

TargetMessage TargetGenerator::generate() {
  const TargetConfig &tc = config.target;

  Target tgt = {
      .value = static_cast<OperationType>(
          rand() % static_cast<int>(OperationType::COUNT)),
      .id = serial_id++,
      .required_preparation =
          rand() % (tc.preparation_time_max - tc.preparation_time_min + 1) +
          tc.preparation_time_min,
      .difficulty = rand() % (tc.difficulty_max - tc.difficulty_min + 1) +
                    tc.difficulty_min,
      .obscurity =
          rand() % (tc.obscurity_max - tc.obscurity_min + 1) + tc.obscurity_min,
      .reward = rand() % (tc.reward_max - tc.reward_min + 1) + tc.reward_min,
      .xp = rand() % (tc.xp_max - tc.xp_min + 1) + tc.xp_min};

  TargetMessage message = {.mType = 1, .target = tgt};

  return message;
}
void TargetGenerator::run() {
  while (true) {
    TargetMessage message = generate();

    if (msgsnd(msq_id, &message, sizeof(TargetMessage) - sizeof(long), 0) ==
        -1) {
      perror("msgsnd failed");
    }

    sleep(config.target.target_generation_delay); // Assume you have this field
  }
}
