#include "MemberGeneratorMessage.h"
#include "GangMember.h"
#include <unistd.h> // for usleep

// Constructor
GangMember::GangMember(int ID, int rank, int trust)
    : ID(ID), rank(rank), trust(trust), preparation_counter(0), ready(false), 
    thread_running(false), should_stop(false) {
      // Initialize mutex and condition variable
      pthread_mutex_init(&counter_mutex, nullptr);
      pthread_cond_init(&counter_cond, nullptr);
    }

GangMember::GangMember(const GangMemberMessage &msg)
    : ID(msg.ID), rank(msg.rank), trust(msg.trust), preparation_counter(0), 
      ready(false), thread_running(false), should_stop(false) {
        pthread_mutex_init(&counter_mutex, nullptr);
        pthread_cond_init(&counter_cond, nullptr);
      }

// Destructor
GangMember::~GangMember() {
    stopPreparationThread();
    pthread_mutex_destroy(&counter_mutex);
    pthread_cond_destroy(&counter_cond);
}

// Getter for ID
int GangMember::getID() const { return ID; }

// Getter for rank
int GangMember::getRank() const { return rank; }

// Setter for rank
void GangMember::setRank(int newRank) { rank = newRank; }

// Simulates some preparation logic
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

// Start the preparation thread (now private)
void GangMember::startPreparationThread() {
    pthread_mutex_lock(&counter_mutex);
    if (!thread_running) {
        should_stop = false;
        if (pthread_create(&preparation_thread, nullptr, preparationThreadFunction, this) == 0) {
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
void* GangMember::preparationThreadFunction(void* arg) {
    GangMember* member = static_cast<GangMember*>(arg);
    member->runPreparationLoop();
    return nullptr;
}

// The actual preparation loop running in the separate thread
void GangMember::runPreparationLoop() {
    pthread_mutex_lock(&counter_mutex);
    
    while (!should_stop) {
        // Wait for work (prepare() was called) or stop signal
        while (preparation_counter <= 0 && !should_stop) {
            pthread_cond_wait(&counter_cond, &counter_mutex);
        }
        
        if (should_stop) break;
        
        // Do the actual work
        if (preparation_counter > 0) {
            pthread_mutex_unlock(&counter_mutex); // Release lock during sleep
            
            usleep(100000); // Sleep for 100ms (simulating work)
            
            pthread_mutex_lock(&counter_mutex); // Re-acquire lock
            
            preparation_counter--;
            if (preparation_counter <= 0) {
                ready = true;
            }
        }else {
            // No waiting - just keep looping
            pthread_mutex_unlock(&counter_mutex);
            usleep(100000); // Sleep even when no work
            pthread_mutex_lock(&counter_mutex);
        }
    }
    
    pthread_mutex_unlock(&counter_mutex);
}


// Check if ready
bool GangMember::isReady() const { return ready; }
