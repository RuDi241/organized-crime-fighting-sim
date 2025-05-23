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

  int memberGeneratorMsqID = -1;
  int targetGeneratorMsqID = -1;
  std::vector<pid_t> children;

  template <typename Component> int spawnComponent();

  void cleanupQueue(int msq_id);
};

#endif
