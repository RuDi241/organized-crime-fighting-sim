#include "Config.h"
#include <cstdio>
#include <unistd.h>
#include <Game.h>
#include "Graphics.h"
#include "VisualizationMSQ.h"

int main() {
  Config config;
  config.load_from_file("Config.json");
  Game game(config);

  // Create a message queue and set it here.
  VisualizationMSQ::msqid = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
  if (VisualizationMSQ::msqid == -1) {
    perror("Failed to create visulaization MSQ");
    exit(1);
  }

  pid_t pid = fork();
  if (pid < 0) {
    perror("Failed to fork");
  } else if (pid == 0) {
    Graphics graphics;
    graphics.run();
  }

  game.run();

  msgctl(VisualizationMSQ::msqid, IPC_RMID, nullptr);
  return 0;
}
