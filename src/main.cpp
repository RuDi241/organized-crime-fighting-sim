#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h> // Include for pid_t

#include "message_queue.h"
#include "Gang.h"
#include "GangMember.h"
#include "Target.h"
#include "GangMember.h"
#include "Game.h"
#include "Config.h"
#include "TargetGenerator.h"
#include <vector>

int memberGenerator_msid, targetGenerator_msid, police_msid, graphics_msid;
std::vector<Gang> gangs;

int main() {

  // read config file.
  Config config;
  config.load_from_file("Config.json");

  // memberGenerator_msid = create_message_queue(MSG_KEY_MEMBER_GENERATOR);
  // targetGenerator_msid = create_message_queue(MSG_KEY_TARGET_GENERATOR);
  // police_msid = create_message_queue(MSG_KEY_POLICE);
  // graphics_msid = create_message_queue(MSG_KEY_GRAPHICS);

  // if (memberGenerator_msid == -1 || targetGenerator_msid == -1 || police_msid
  // == -1 || graphics_msid == -1) {
  //   std::cerr << "Failed to create message queues." << std::endl;
  //   return EXIT_FAILURE;
  // }

  return 0;
}
