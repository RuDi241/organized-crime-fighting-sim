#include "Game.h"
#include "MemberGenerator.h"
#include "TargetGenerator.h"
#include "Police.h"
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <unistd.h>

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
  cleanupQueue(memberGeneratorMsqID);
  cleanupQueue(targetGeneratorMsqID);
  cleanupQueue(policeArrestGangMsqID);
  cleanupQueue(agentToPoliceMsqID);
  for (pid_t pid : children) {
    waitpid(pid, nullptr, 0);
  }
}

void Game::run() {}

int Game::initMsq(){
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

//overloaded spawnComponent to also handle the case when a component also listens to queue
template <typename Component> void Game::spawnComponent(int msq_id, int receive_msq_id) {
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
