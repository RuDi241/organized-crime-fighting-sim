#include "Game.h"
#include "MemberGenerator.h"
#include "TargetGenerator.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

Game::Game(const Config &config) : config(config) {
  member_generator_msq_id = spawnComponent<MemberGenerator>();
  target_generator_msq_id = spawnComponent<TargetGenerator>();
}

Game::~Game() {
  cleanupQueue(member_generator_msq_id);
  cleanupQueue(target_generator_msq_id);
  for (pid_t pid : children) {
    waitpid(pid, nullptr, 0);
  }
}

void Game::run() {}

template <typename Component> int Game::spawnComponent() {
  int msq_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
  if (msq_id == -1) {
    perror("msgget failed");
    exit(EXIT_FAILURE);
  }

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

  return msq_id;
}

void Game::cleanupQueue(int msq_id) {
  if (msq_id != -1) {
    msgctl(msq_id, IPC_RMID, nullptr);
  }
}
