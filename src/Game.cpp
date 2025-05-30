#include "Game.h"
#include "MemberGenerator.h"
#include "TargetGenerator.h"
#include "VisualizationMSQ.h"
#include "Police.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>
#include "utils.h"
#include "Gang.h"
#include <csignal>
#include <iostream>

volatile sig_atomic_t running = 1;

extern "C" void handle_sigterm(int signum) {
  running = 0;
  write(STDOUT_FILENO, "Game is terminating...\n", 24);
  _exit(0);
}

Game::Game(const Config &config) : config(config) {
  memberGeneratorMsqID = initMsq();
  targetGeneratorMsqID = initMsq();
  policeArrestGangMsqID = initMsq();
  agentToPoliceMsqID = initMsq();
  spawnComponent<MemberGenerator>(memberGeneratorMsqID, agentToPoliceMsqID);
  spawnComponent<TargetGenerator>(targetGeneratorMsqID);
  spawnComponent<Police>(policeArrestGangMsqID, agentToPoliceMsqID);
}

Game::~Game() {
  for (pid_t pid : children) {
    kill(pid, SIGKILL); // Send termination signal to all child processes
    waitpid(pid, nullptr, 0);
  }
  cleanupQueue(memberGeneratorMsqID);
  cleanupQueue(targetGeneratorMsqID);
  cleanupQueue(policeArrestGangMsqID);
  cleanupQueue(agentToPoliceMsqID);
}

void Game::run() {
  // signal
  std::signal(SIGTERM, handle_sigterm);

  // fork gangs
  // int numberOfGangs = random_int(config.gang.num_gangs_min,
  // config.gang.num_gangs_max);
  int numberOfGangs = 1; // For testing purposes, set to 1 gang
  for (int i = 0; i < numberOfGangs; ++i) {
    pid_t pid = fork();
    if (pid == 0) {
      // CHILD PROCESS
      int capacity =
          random_int(config.gang.num_members_min, config.gang.num_members_max);
      std::cout << "Capacity at GAME RUN: " << capacity << std::endl;
      int acceptanceRate =
          random_int(1, 100); // Acceptance rate between 1 and 100
      Gang gang(config, i + 1, capacity, acceptanceRate, memberGeneratorMsqID,
                targetGeneratorMsqID, policeArrestGangMsqID);
      gang.run();
      _exit(0);
    } else if (pid > 0) {
      children.push_back(pid);
    } else {
      perror("fork failed");
      exit(EXIT_FAILURE);
    }
  }

  while (running) {
    // ... main game loop ...
    sleep(1);
    // cont++;
    //  if (cont > 10) { // For testing purposes, run for 10 seconds
    //    running = 0;
    //  }
  }

  std::cout << "Game loop ended. Cleaning up..." << std::endl;
  // for (pid_t pid : children) {
  //   kill(pid, SIGKILL); // Send termination signal to all child processes
  //   waitpid(pid, nullptr, 0);
  // }
}

int Game::initMsq() {
  int msq_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
  if (msq_id == -1) {
    perror("msgget failed");
    exit(EXIT_FAILURE);
  }

  return msq_id;
}
template <typename Component> void Game::spawnComponent(int msq_id) {
  pid_t pid = fork();
  if (pid == 0) {
    // CHILD PROCESS
    Component instance(config, msq_id);
    instance.run();
    _exit(0);
  } else if (pid > 0) {
    children.push_back(pid);
  } else {
    perror("fork failed");
    exit(EXIT_FAILURE);
  }
}

// overloaded spawnComponent to also handle the case when a component also
// listens to queue
template <typename Component>
void Game::spawnComponent(int msq_id, int receive_msq_id) {
  pid_t pid = fork();
  if (pid == 0) {
    Component instance(config, msq_id, receive_msq_id);
    instance.run();
    _exit(0);
  } else if (pid > 0) {
    children.push_back(pid);
  } else {
    perror("fork failed");
    exit(EXIT_FAILURE);
  }
}

void Game::cleanupQueue(int msq_id) {
  if (msq_id != -1) {
    msgctl(msq_id, IPC_RMID, nullptr);
  }
}
