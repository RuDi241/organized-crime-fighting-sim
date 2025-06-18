#include "MemberGeneratorMessage.h"
#include "GangMember.h"
#include <unistd.h>
#include "VisualizationMSQ.h"
GangMember::GangMember(int ID, int rank, int trust, int gangID)
    : ID(ID), gangID(gangID), rank(rank), trust(trust), preparation_counter(0),
      ready(false), thread_running(false), should_stop(false) {
  // Initialize mutex and condition variable
  pthread_mutex_init(&counter_mutex, nullptr);
  pthread_cond_init(&counter_cond, nullptr);
}

GangMember::GangMember(const GangMemberMessage &msg, int gangID)
    : ID(msg.ID), gangID(gangID), rank(msg.rank), trust(msg.trust),
      preparation_counter(0), ready(false), thread_running(false),
      should_stop(false) {
  pthread_mutex_init(&counter_mutex, nullptr);
  pthread_cond_init(&counter_cond, nullptr);
}

int GangMember::getPreparation() { return preparation_counter; }
GangMember::~GangMember() {
  pthread_mutex_destroy(&counter_mutex);
  pthread_cond_destroy(&counter_cond);
}

int GangMember::getID() const { return ID; }

int GangMember::getRank() const { return rank; }

int GangMember::getTrust() const { return trust; }

void GangMember::setTrust(int newTrust) { trust = newTrust; }

void GangMember::setRank(int newRank) { rank = newRank; }

// void GangMember::receiveInformation(InformationMessage message)  {
//     // Base implementation does nothing - normal members just receive info
//     // This is virtual so derived classes can override
//     std::cout << "GangMember received message with ID (should not print
//     here): " << message.MessageID << std::endl;
// }

// Simple preparation function - just sets up the counter
void GangMember::prepare() {
  preparation_counter = trust;
  ready = false;
}

// The actual preparation work - this will be called from a thread in Gang
void GangMember::runPreparation() {
  std::cout << "Member " << ID << " starting preparation with counter: " << preparation_counter << std::endl;
  
  // Count down without any synchronization - each member has its own thread
  while (preparation_counter > 0) {
    usleep(1000000); // Sleep for 1 second
    preparation_counter--;
    std::cout << "Member " << ID << " preparation counter: " << preparation_counter << std::endl;
  }
  
  ready = true;
  std::cout << "Member " << ID << " is ready!" << std::endl;
}

// Static thread function for Gang to use
void* GangMember::preparationThreadFunction(void* arg) {
  GangMember* member = static_cast<GangMember*>(arg);
  member->runPreparation();
  return nullptr;
}

bool GangMember::isReady() const { return ready; }
