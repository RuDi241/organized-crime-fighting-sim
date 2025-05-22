#define _POSIX_C_SOURCE 200809L

#include "MemberGenerator.h"
#include "Police.h"
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <time.h>
#include <sys/types.h> // Include for pid_t

#include "message_queue.h"
#include "Gang.h"
#include "GangMember.h"
#include "Target.h"
#include "GangMember.h"
#include "Game.h"
#include "Config.h"
#include "TargetGenerator.h"
#include <vector>
#include "utils.h"

// Flag to indicate if the program should continue running
static volatile int running = 1;

// Array to store child process PIDs for cleanup
static pid_t *child_pids = NULL;
static int child_count = 0;
static pid_t graphics_pid;

static int memberGenerator_msid;
static int targetGenerator_msid;
static int police_msid;
static int graphics_msid;

std::vector<Gang> gangs;
Police police;
MemberGenerator memberGenerator;
TargetGenerator targetGenerator;

// Function to cleanup all child processes
static void cleanup_children() {
  if (child_pids) {
    printf("Terminating %d child processes...\n", child_count);
    for (int i = 0; i < child_count; i++) {
      if (child_pids[i] > 0) {
        printf("Sending SIGTERM to process %d\n", child_pids[i]);
        kill(child_pids[i], SIGTERM);
      }
    }

    // Wait for all children to terminate
    printf("Waiting for all child processes to terminate...\n");
    for (int i = 0; i < child_count; i++) {
      if (child_pids[i] > 0) {
        int status;
        waitpid(child_pids[i], &status, 0);
        printf("Process %d terminated with status %d\n", child_pids[i], status);
      }
    }

    free(child_pids);
    child_pids = NULL;
    child_count = 0;
  }
}

void destroy_resources_handler(int signum) {
  running = 0;
  kill(graphics_pid, SIGTERM);
  // Cleanup
  cleanup_children();
  // Destroy message queues
  destroy_message_queue(memberGenerator_msid);
  destroy_message_queue(targetGenerator_msid);
  destroy_message_queue(police_msid);
  destroy_message_queue(graphics_msid);

  running = 0;
  printf("\nMain process received signal %d, preparing to terminate all "
         "processes...\n",
         signum);
}

// Function to add a child PID to the tracking array
static void add_child_pid(pid_t pid) {
  child_count++;
  child_pids =
      static_cast<pid_t *>(realloc(child_pids, child_count * sizeof(pid_t)));
  if (child_pids) {
    child_pids[child_count - 1] = pid;
  } else {
    fprintf(stderr, "Failed to allocate memory for child PID tracking\n");
    exit(EXIT_FAILURE);
  }
}

int main() {
  // Set up signal handlers
  signal(SIGUSR1, &destroy_resources_handler);

  // Initialize random number generator
  seed_random();

  // read config file.

  memberGenerator_msid = create_message_queue(MSG_KEY_MEMBER_GENERATOR);
  targetGenerator_msid = create_message_queue(MSG_KEY_TARGET_GENERATOR);
  police_msid = create_message_queue(MSG_KEY_POLICE);
  graphics_msid = create_message_queue(MSG_KEY_GRAPHICS);

  if (memberGenerator_msid == -1 || targetGenerator_msid == -1 ||
      police_msid == -1 || graphics_msid == -1) {
    std::cerr << "Failed to create message queues." << std::endl;
    return EXIT_FAILURE;
  }

  // Initialize gangs (example: create 3 gangs)
  gangs.push_back(
      Gang(1, 10, 80)); // Gang ID 1, capacity 10, acceptance rate 80%
  gangs.push_back(
      Gang(2, 15, 70)); // Gang ID 2, capacity 15, acceptance rate 70%
  gangs.push_back(Gang(3, 8, 90)); // Gang ID 3, capacity 8, acceptance rate 90%

  std::cout
      << "Main process: All objects instantiated. Starting to fork processes..."
      << std::endl;

  // fork the graphics process
  graphics_pid = fork();
  if (graphics_pid == 0) {
    // Child process - Graphics
    std::cout << "Graphics process started (PID: " << getpid() << ")"
              << std::endl;
    // Graphics run logic would go here
    // For now, just simulate with a loop
    // while (running) {
    //   sleep(2);
    //   cout << "Graphics process running..." << endl;
    // }
    exit(EXIT_SUCCESS);
  } else if (graphics_pid > 0) {
    add_child_pid(graphics_pid);
    std::cout << "Main: Forked graphics process with PID " << graphics_pid
              << std::endl;
  } else {
    perror("Failed to fork graphics process");
    return EXIT_FAILURE;
  }

  // fork the member generator process
  pid_t memberGen_pid = fork();
  if (memberGen_pid == 0) {
    // Child process - Member Generator
    std::cout << "Member Generator process started (PID: " << getpid() << ")"
              << std::endl;
    memberGenerator.run();
    std::cout << "Member Generator process exiting..." << std::endl;
    exit(EXIT_SUCCESS);
  } else if (memberGen_pid > 0) {
    add_child_pid(memberGen_pid);
    std::cout << "Main: Forked member generator process with PID "
              << memberGen_pid << std::endl;
  } else {
    perror("Failed to fork member generator process");
    return EXIT_FAILURE;
  }

  // fork the target generator process
  pid_t targetGen_pid = fork();
  if (targetGen_pid == 0) {
    // Child process - Target Generator
    std::cout << "Target Generator process started (PID: " << getpid() << ")"
              << std::endl;
    targetGenerator.run();
    std::cout << "Target Generator process exiting..." << std::endl;
    exit(EXIT_SUCCESS);
  } else if (targetGen_pid > 0) {
    add_child_pid(targetGen_pid);
    std::cout << "Main: Forked target generator process with PID "
              << targetGen_pid << std::endl;
  } else {
    perror("Failed to fork target generator process");
    return EXIT_FAILURE;
  }

  // fork the police process
  pid_t police_pid = fork();
  if (police_pid == 0) {
    // Child process - Police
    std::cout << "Police process started (PID: " << getpid() << ")"
              << std::endl;
    police.run();
    exit(EXIT_SUCCESS);
    std::cout << "Police process exiting..." << std::endl;
  } else if (police_pid > 0) {
    add_child_pid(police_pid);
    std::cout << "Main: Forked police process with PID " << police_pid
              << std::endl;
  } else {
    perror("Failed to fork police process");
    return EXIT_FAILURE;
  }

  // fork gang processes
  for (size_t i = 0; i < gangs.size(); i++) {
    pid_t gang_pid = fork();
    if (gang_pid == 0) {
      // Child process - Gang
      std::cout << "Gang " << gangs[i].getID()
                << " process started (PID: " << getpid() << ")" << std::endl;
      gangs[i].run();
      std::cout << "Gang " << gangs[i].getID() << " process exiting..."
                << std::endl;
      exit(EXIT_SUCCESS);
    } else if (gang_pid > 0) {
      add_child_pid(gang_pid);
      std::cout << "Main: Forked gang " << gangs[i].getID()
                << " process with PID " << gang_pid << std::endl;
    } else {
      perror("Failed to fork gang process");
      return EXIT_FAILURE;
    }
  }

  std::cout << "Main process: All child processes forked successfully. Total "
               "children: "
            << child_count << std::endl;

  // Main process monitoring loop
  while (running) {
    sleep(1);
    std::cout << "Main process monitoring... (PID: " << getpid() << ")"
              << std::endl;
  }

  destroy_resources_handler(SIGUSR1);
  printf("Main process exiting...\n");

  return 0;
}
