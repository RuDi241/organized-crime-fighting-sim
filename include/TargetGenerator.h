
#ifndef TARGET_GENERATOR_H
#define TARGET_GENERATOR_H

#include "Config.h"
#include "Target.h"

class TargetGenerator {
public:
  TargetGenerator() = delete;
  TargetGenerator(const Config &config, int msq_id);
  static Target messageToTarget(const TargetMessage &message);

  void run();

private:
  TargetMessage generate();

  const Config &config;
  int msq_id;
  int serial_id = 1;
};

#endif
