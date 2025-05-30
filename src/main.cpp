#include "Config.h"
#include <unistd.h>
#include <Game.h>
#include "Graphics.h"
#include "VisualizationMSQ.h"

int main() {
  Config config;
  config.load_from_file("Config.json");
  Game game(config);

  // Create a message queue and set it here.
  VisualizationMSQ::msqid = -1;

  game.run();
  pid_t pid = fork();
  if (pid == 0) {
    Graphics graphics;
    graphics.run();
  }
  return 0;
}
