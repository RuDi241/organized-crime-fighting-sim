#include <unistd.h>
#include "Config.h"
#include <iostream>

int main() {

  Config config;
  config.load_from_file("Config.json");
  return 0;
}
