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
  stopPreparationThread();
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

void GangMember::prepare() {
  pthread_mutex_lock(&counter_mutex);

  // Start thread if not already running
  if (!thread_running) {
    should_stop = false;
    pthread_mutex_unlock(&counter_mutex); // Release lock before creating thread
    startPreparationThread();
    pthread_mutex_lock(&counter_mutex); // Re-acquire lock
  }

  preparation_counter = trust; // Set initial counter value based on trust
  ready = false;
  pthread_cond_signal(&counter_cond); // Wake up the preparation thread
  pthread_mutex_unlock(&counter_mutex);
}

void GangMember::startPreparationThread() {
  pthread_mutex_lock(&counter_mutex);
  if (!thread_running) {
    should_stop = false;
    if (pthread_create(&preparation_thread, nullptr, preparationThreadFunction,
                       this) == 0) {
      thread_running = true;
    }
  }
  pthread_mutex_unlock(&counter_mutex);
}

// Stop the preparation thread
void GangMember::stopPreparationThread() {
  pthread_mutex_lock(&counter_mutex);
  if (thread_running) {
    should_stop = true;
    pthread_cond_signal(&counter_cond); // Wake up thread so it can exit
    pthread_mutex_unlock(&counter_mutex);

    pthread_join(preparation_thread, nullptr); // Wait for thread to finish

    pthread_mutex_lock(&counter_mutex);
    thread_running = false;
  }
  pthread_mutex_unlock(&counter_mutex);
}

// Static thread function
void *GangMember::preparationThreadFunction(void *arg) {
  GangMember *member = static_cast<GangMember *>(arg);
  member->runPreparationLoop();
  return nullptr;
}

// The actual preparation loop running in the separate thread
void GangMember::runPreparationLoop() {
  pthread_mutex_lock(&counter_mutex);

  while (!should_stop) {
    while (preparation_counter <= 0 && !should_stop) {
      pthread_cond_wait(&counter_cond, &counter_mutex);
    }

    if (should_stop)
      break;
    if (preparation_counter > 0) {
      pthread_mutex_unlock(&counter_mutex);

      usleep(100000);

      pthread_mutex_lock(&counter_mutex);

      preparation_counter--;

      if (preparation_counter <= 0) {
        ready = true;
      }
    } else {
      pthread_mutex_unlock(&counter_mutex);
      usleep(100000);
      pthread_mutex_lock(&counter_mutex);
    }
  }

  pthread_mutex_unlock(&counter_mutex);
}

bool GangMember::isReady() const { return ready; }
