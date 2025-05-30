#include "Config.h"
#include <unistd.h>
#include <Game.h>
#include "VisualizationMSQ.h"

int main() {
  Config config;
  config.load_from_file("Config.json");
  Game game(config);

  // Create a message queue and set it here.
  VisualizationMSQ::msqid = -1;

  game.run();

  return 0;
}
