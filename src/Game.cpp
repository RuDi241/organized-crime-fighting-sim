#include "Game.h"
#include "MemberGenerator.h"
#include "TargetGenerator.h"
#include "Police.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

Game::Game(const Config &config) : config(config) {
  memberGeneratorMsqID = spawnComponent<MemberGenerator>();
  targetGeneratorMsqID = spawnComponent<TargetGenerator>();
  policeArrestGangMsqID = spawnComponent<Police>(agentToPoliceMsqID);
}

Game::~Game() {
  cleanupQueue(memberGeneratorMsqID);
  cleanupQueue(targetGeneratorMsqID);
  cleanupQueue(policeArrestGangMsqID);
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

//overloaded spawnComponent to also handle the case when a component also listens to queue
template <typename Component> int Game::spawnComponent(int receive_msq_id) {
  int msq_id = msgget(IPC_PRIVATE, IPC_CREAT | 0666);
  if (msq_id == -1) {
    perror("msgget failed");
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();
  if (pid == 0) {
    // CHILD PROCESS
    Component instance(config, msq_id, receive_msq_id);
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
