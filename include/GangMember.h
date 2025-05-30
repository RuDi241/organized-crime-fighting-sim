#ifndef GANG_MEMBER_H
#define GANG_MEMBER_H

#include "MemberGeneratorMessage.h"
#include <pthread.h>
#include "InformationMessage.h"
#include <vector>

class GangMember {
public:
  GangMember(int ID, int rank, int trust);
  GangMember(const GangMemberMessage &msg);
  GangMember() = delete; // Disable default constructor
  virtual ~GangMember(); // Destructor to clean up thread resources

  int getID() const;
  int getRank() const;
  int getTrust() const;
  void setRank(int rank);
  void prepare();
  bool isReady() const;
  virtual void receiveInformation(InformationMessage message);

protected: // Changed from private to allow derived class access
  int ID;
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
  static void *preparationThreadFunction(void *arg);
  void runPreparationLoop();
};

#endif
