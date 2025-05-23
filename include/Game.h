#ifndef GAME_H
#define GAME_H

#include "Config.h"
#include <vector>
#include <unistd.h>

class Game {
public:
  explicit Game(const Config &config);
  ~Game();

  void run();

private:
  const Config &config;

  int member_generator_msq_id = -1;
  std::vector<pid_t> children;

  template <typename Component> int spawnComponent();

  void cleanupQueue(int msq_id);
};

#endif
