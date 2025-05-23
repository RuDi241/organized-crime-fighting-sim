#include "Config.h"
#include <unistd.h>
#include <Game.h>

int main() {
  Config config;
  config.load_from_file("Config.json");
  Game game;

  return 0;
}
