#ifndef GANG_MEMBER_H
#define GANG_MEMBER_H

#include "MemberGeneratorMessage.h"
#include <pthread.h>

class GangMember {
public:
  GangMember(int ID, int rank, int trust);
  GangMember(const GangMemberMessage &msg);
  GangMember() = delete; // Disable default constructor
  ~GangMember(); // Destructor to clean up thread resources

  int getID() const;
  int getRank() const;
  void setRank(int rank);
  void prepare();
  bool isReady() const;
  
  // Thread management (private - handled internally)
private:
  void startPreparationThread();
  void stopPreparationThread();
  // Static thread function (must be static for pthread_create)
  static void* preparationThreadFunction(void* arg);
  // Instance method called by the static function
  void runPreparationLoop();
private:
  int ID;
  int rank;
  int trust;
  int preparation_counter = 0; 
  bool ready = false; // Replaces int isReady for clarity

    // Thread-related members
  pthread_t preparation_thread;
  pthread_mutex_t counter_mutex;
  pthread_cond_t counter_cond;
  bool thread_running = false;
  bool should_stop = false;
};

#endif
