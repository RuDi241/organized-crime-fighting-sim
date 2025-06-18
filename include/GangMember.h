#ifndef GANG_MEMBER_H
#define GANG_MEMBER_H

#include "MemberGeneratorMessage.h"
#include <pthread.h>
#include "InformationMessage.h"
#include <vector>
#include <iostream>
class GangMember {
public:
  GangMember(int ID, int rank, int trust, int gangID);
  GangMember(const GangMemberMessage &msg, int gangID);
  GangMember() = delete; // Disable default constructor
  virtual ~GangMember(); // Destructor to clean up thread resources

  int getID() const;
  int getRank() const;
  int getTrust() const;
  void setTrust(int newTrust);
  void setRank(int rank);
  void prepare();
  void runPreparation();
  bool isReady() const;
  int getPreparation();
  virtual void receiveInformation(InformationMessage message) {
    // Base implementation does nothing - normal members just receive info
    // This is virtual so derived classes can override
    std::cout << "GangMember received message with ID (should not print here): "
              << message.MessageID << std::endl;
  }
  static void* preparationThreadFunction(void* arg);

protected: // Changed from private to allow derived class access
  int ID;
  int gangID;
  int rank;
  int trust;
  int preparation_counter = 0;
  bool ready = false; // Replaces int isReady for clarity
  pthread_t preparation_thread;
  pthread_mutex_t counter_mutex;
  pthread_cond_t counter_cond;
  bool thread_running = false;
  bool should_stop = false;
  std::vector<InformationMessage> infoList;

private:
  void startPreparationThread();
  void stopPreparationThread();
  void runPreparationLoop();
};

#endif
