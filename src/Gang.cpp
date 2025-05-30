#include "Gang.h"
#include "Config.h"
#include <iostream>
#include <sys/msg.h>
#include "MemberGenerator.h"
#include <SecretAgent.h>
#include <NormalGangMember.h>
// Constructor
Gang::Gang(const Config &config, int ID, int capacity, int acceptance_rate, int member_generator_msq_id)
    : config(config), ID(ID), capacity(capacity),
      acceptance_rate(acceptance_rate), memberGeneratorMsqID(member_generator_msq_id) {
  GangMembers.reserve(capacity); // Optional: optimize memory allocation
}

// Accept a new member into the gang
void Gang::acceptMember() {
  if (GangMembers.size() >= static_cast<size_t>(capacity)) {
    std::cout << "Gang is full. Member rejected.\n";
    return;
  }
  
  GangMemberMessage newMember;
  ssize_t messageSize = msgrcv(memberGeneratorMsqID, &newMember, sizeof(newMember)- sizeof(long),0, IPC_NOWAIT);
  if (messageSize == -1) {
    perror("msgrcv failed.");
  }else if(messageSize > 0) {
    //GangMember generatedMember = MemberGenerator::messageToMember(newMember);
  }
  

  // Optional: simulate acceptance probability (basic version)
  //GangMembers.push_back(gangMember);
  std::cout << "GangMember accepted. Total now: " << GangMembers.size() << "\n";
}

// GangMember messageToMember(const GangMemberMessage &msg) {
//   if (msg.type == GangMemberType::SECRET_AGENT) {
//     SecretAgent member(msg, this->memberToPoliceMsqID);
//     return member;
//   } else {
    
//     NormalGangMember member(msg);
//     return member;
//   }
// }

void Gang::informGangMembers() {
  InformationMessage message;
  message.MessageID = messageIdGenerator++;
  message.gangID = ID;
  message.weight = random_int(config.info.weight_min, config.info.weight_max);
  float trueProb = random_float(0, 1.0f);
  if (trueProb > config.info.p_true) {
    message.weight = -message.weight;
  }

  for (GangMember &member : GangMembers) {
    float spreadProb = random_float(0, 1.0f);
    if (spreadProb < config.info.p_spread) {
      member.receiveInformation(message);
    }
  }
}

// Simulate the gang starting operations
void Gang::startOperation() {
  bool allReady = true;
  for (const GangMember &member : GangMembers) {
    if (!member.isReady()) {
      allReady = false;
      break;
    }
  }

  if (allReady) {
    InformationMessage message;
    message.MessageID = messageIdGenerator++;
    message.gangID = ID;
    message.type = InformationMessageType::ATTACK;
    
    for (GangMember &member : GangMembers) {
      member.receiveInformation(message);
    }
    std::cout << "Gang " << ID << " starting operation with "
          << GangMembers.size() << " members.\n";
  }else {
    std::cout << "Gang " << ID << "CANNOT starting operation yet "
          << std::endl;
  }

}

// Simulate gang members leaving jail
void Gang::leaveJail() {
  std::cout << "Gang " << ID << " members leaving jail.\n";
  // Placeholder logic
}

// High-level orchestration
void Gang::run() {
  std::cout << "Running Gang #" << ID << "\n";
  startOperation();
  leaveJail();
}

int Gang::getID() const { return ID; }

int Gang::getCapacity() const { return capacity; }

int Gang::getAcceptanceRate() const { return acceptance_rate; }
