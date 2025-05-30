#include "Gang.h"
#include "Config.h"
#include <iostream>
#include <sys/msg.h>
#include "MemberGenerator.h"
#include <SecretAgent.h>
#include <NormalGangMember.h>
#include "ArrestMessage.h"
#include <csignal>
#include <iostream>


// Constructor
Gang::Gang(const Config &config, int ID, int capacity, int acceptance_rate, int member_generator_msq_id, 
           int target_generator_msq_id, int police_arrest_gang_msq_id)
    : config(config), ID(ID), capacity(capacity),
      acceptance_rate(acceptance_rate), memberGeneratorMsqID(member_generator_msq_id),
      targetGeneratorMsqID(target_generator_msq_id), policeArrestGangMsqID(police_arrest_gang_msq_id) {
  GangMembers.reserve(capacity); // Optional: optimize memory allocation
}

void Gang::acceptMember() {
  if (GangMembers.size() >= static_cast<size_t>(capacity)) {
    std::cout << "Gang is full. Member rejected." << std::endl;
    return;
  }
  
  GangMemberMessage newMember;
  ssize_t messageSize = msgrcv(memberGeneratorMsqID, &newMember, sizeof(newMember) - sizeof(long), 0, IPC_NOWAIT);
  if (messageSize == -1) {
    // perror("msgrcv failed.");
  } else if (messageSize > 0) {
    int temp = 0;
    if (newMember.type == GangMemberType::SECRET_AGENT) {
      temp = 69;
    }
    std::cout << "Type of member: " << temp << std::endl;
    // Directly push the unique_ptr returned by messageToMember
    GangMembers.push_back(MemberGenerator::messageToMember(newMember));
    std::cout << "New member accepted: ID = " << GangMembers.back()->getID() 
              << ", Rank = " << GangMembers.back()->getRank() << std::endl;
    std::cout << "GangMember accepted. Total now: " << GangMembers.size() << std::endl;
  }
}

void Gang::informGangMembers() {
  InformationMessage message;
  message.MessageID = messageIdGenerator++;
  message.gangID = ID;
  message.weight = random_int(config.info.weight_min, config.info.weight_max);
  float trueProb = random_float(0, 1.0f);
  if (trueProb > config.info.p_true) {
    message.weight = -message.weight;
  }

  for (auto &member : GangMembers) {
    float spreadProb = random_float(0, 1.0f);
    if (spreadProb < config.info.p_spread) {
      member->receiveInformation(message);
    }
  }

  std::cout << "Information element sent to gang members. Gang ID: " 
            << ID << ", Weight: " << message.weight << std::endl;
}

bool checkAllReady(const std::vector<std::unique_ptr<GangMember>> &members) {
  for (const auto &member : members) {
    if (!member->isReady()) {
      std::cout << "Gang member " << member->getID() << " is not ready." << std::endl;
      std::cout << "Gang cannot start operation yet." << std::endl;
      return false;
    }
  }
  return true;
}

void Gang::pickTarget() {
  // Try to receive a target from the message queue (non-blocking)
  // DONT FORGET TO PASS THE ID OR THE TARGET MESSAGE QUEUE ID.
  TargetMessage targetMsg;
  ssize_t msgSize = msgrcv(targetGeneratorMsqID, &targetMsg, sizeof(TargetMessage) - sizeof(long), 0, IPC_NOWAIT);
  if (msgSize > 0) {
    targets.push_back(targetMsg.target);
    std::cout << "Gang " << ID << " picked target with id: " << targetMsg.target.id << std::endl;
  } else {
    std::cout << "Gang " << ID << " did not receive a new target." << std::endl;
  }
}

void Gang::startOperation() {
  InformationMessage message;
  message.MessageID = messageIdGenerator++;
  message.gangID = ID;
  message.type = InformationMessageType::ATTACK;
    
  for (auto &member : GangMembers) {
    member->receiveInformation(message);
  }
  std::cout << "Gang " << ID << " starting operation with "
            << GangMembers.size() << " members.\n";
}

// Simulate gang members leaving jail
void Gang::leaveJail() {
  std::cout << "Gang " << ID << " members leaving jail.\n";
}

void Gang::prepareAll() {
  for (auto &member : GangMembers) {
    member->prepare();
  }
  std::cout << "Gang " << ID << " members are preparing for the operation." << std::endl;
}

void Gang::waitAllReady() {
  bool allReady = false;
  while (!allReady) {
    allReady = true;
    for (const auto &member : GangMembers) {
      if (!member->isReady()) {
        allReady = false;
        break;
      }
    }
    if (!allReady) sleep(1);
  }
  std::cout << "All gang members are ready for the operation." << std::endl;
}

bool Gang::checkPoliceCaught() {
  ArrestMessage arrestMsg;
  ssize_t msgSize = msgrcv(policeArrestGangMsqID, &arrestMsg, sizeof(ArrestMessage) - sizeof(long), ID, IPC_NOWAIT);
  if (msgSize > 0) {
    std::cout << "Gang " << ID << " was caught by police. Jail period: " << arrestMsg.arrestPeriod << std::endl;
    return arrestMsg.arrestPeriod;
  }
  return 0; // No arrest message received
}

void Gang::goToJail(int duration) {
  sleep(duration);
  leaveJail();
}

void Gang::investigateAndKill() {
  if (GangMembers.empty()) {
    std::cout << "No gang members to investigate and kill.\n";
    return;
  }
  // Choose the member with minimum trust 
  size_t killIdx = 0;
  for (size_t i = 1; i < GangMembers.size(); ++i) {
    if (GangMembers[i]->getTrust() < GangMembers[killIdx]->getTrust()) {
      killIdx = i;
    }
  }
  std::cout << "Investigating and killing member with ID: "
            << GangMembers[killIdx]->getID() << " (Trust: "
            << GangMembers[killIdx]->getTrust() << ")" << std::endl;
  // Remove the member from the gang
  GangMembers.erase(GangMembers.begin() + killIdx);
}

// High-level orchestration
void Gang::run() {
  std::cout << "Gang cap. : "<< capacity << std::endl;
  while(GangMembers.size() < capacity) {
    acceptMember(); // Accept new members until capacity is reached
  }
  std::cout << "Gang " << ID << "with size: " << GangMembers.size() <<" is ready to start operations.\n";

  while (true) {
    pickTarget();
    for (int i = 0; i < 3; ++i) informGangMembers();
    prepareAll();
    waitAllReady();
    startOperation();
    int jailPeriod = checkPoliceCaught();
    if (jailPeriod > 0) {
      goToJail(jailPeriod); // Example jail time, replace with actual
      investigateAndKill();
      acceptMember(); // Accept new members after killing a member
    } 
    // Loop again
    sleep(3); // Simulate time between operations
  }
}

int Gang::getID() const { return ID; }

int Gang::getCapacity() const { return capacity; }

int Gang::getAcceptanceRate() const { return acceptance_rate; }
