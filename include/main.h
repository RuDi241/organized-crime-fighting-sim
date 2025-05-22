#ifndef MAIN_H
#define MAIN_H

// Define POSIX source compatibility level for semaphore functions
#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
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
#include "cJSON.h"
#include "shader_utils.h"
#include "TargetGenerator.h"
#include "Police.h"
#include "SecretAgent.h"
#include "GangMember.h"
#include "NormalGangMember.h"
#include "MemberGenerator.h"
#include "utils.h"


#endif // MAIN_H
