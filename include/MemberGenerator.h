#ifndef MEMBER_GENERATOR_H
#define MEMBER_GENERATOR_H

#include "Config.h"
class MemberGenerator {
public:
  MemberGenerator() = delete;
  MemberGenerator(const Config &config, int msq_id);
  void run();

private:
  const Config &config;
};

#endif
