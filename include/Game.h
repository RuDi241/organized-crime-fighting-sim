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
  int memberGeneratorMsqID = -1;
  int targetGeneratorMsqID = -1;
  int agentToPoliceMsqID = -1;
  int policeArrestGangMsqID = -1;
  volatile int running = 1; // Flag to control the game loop
  const Config &config;
  //includes ALL processes
  std::vector<pid_t> children; 
  int initMsq();
  template <typename Component> void spawnComponent(int msq_id); 
  template <typename Component> void spawnComponent(int msq_id, int receive_msq_id);
  void cleanupQueue(int msq_id);
};

#endif
