#include "Police.h"
#include <iostream>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>


Police::Police(const Config &config, int msq_id, int receive_msq_id)
    : config(config), msqID(msq_id), agentsMsqID(receive_msq_id){
  srand(time(NULL));
}

int Police::getNumberOfCaughtGangs() {
    return numberOfCaughtGangs;
}

int Police::getNumberOfSuccessfulOperations() {
    return numberOfSuccessfulOperations;
}

void Police::run() {
    while (numberOfCaughtGangs < config.sim_end.gang_plans_thwarted && numberOfSuccessfulOperations < config.sim_end.gang_plans_succeed)
    {
        ssize_t agentMessageSize = msgrcv(agentsMsqID, &agentMessage, sizeof(agentMessage)- sizeof(long),0, IPC_NOWAIT); 
        if (agentMessageSize == -1) {
            //perror("msgrcv failed.");
        } else if (agentMessageSize > 0) {
            std::cout << "Police received message from gang no." << agentMessage.gangID << " with ID: " << agentMessage.MessageID << std::endl;
            if (agentMessage.type == AgentMessageType::NORMAL_INFO) {
                std::cout << "Police received normal info from gang no." << agentMessage.gangID << " with ID: " << agentMessage.MessageID << std::endl;
               // if (infoCounter.find(agentMessage.gangID) == NULL || infoCounter[agentMessage.gangID].find(agentMessage.MessageID) == infoCounter[agentMessage.gangID].end()) {
   //                 infoCounter[agentMessage.gangID][agentMessage.MessageID] = 1;
                //} else {
                    infoCounter[agentMessage.gangID][agentMessage.MessageID]++;
                    std::cout << "Police received info from gang no." << agentMessage.gangID << " with ID: " << agentMessage.MessageID << "count" << infoCounter[agentMessage.gangID][agentMessage.MessageID] << std::endl;
                    if(infoCounter[agentMessage.gangID][agentMessage.MessageID] == 2){
                        std::cout << "Police received 2 info from gang no." << agentMessage.gangID << " with ID: " << agentMessage.MessageID << std::endl;
                        totalGangInfo[agentMessage.gangID] += agentMessage.weight;
                    }
//                }                
            } else if (agentMessage.type == AgentMessageType::ATTACK) {
                std::cout << "Police received attack message from gang no." << agentMessage.gangID << " with ID: " << agentMessage.MessageID << std::endl;
                catchGang(agentMessage.gangID);
            } else {
                //sleep(1);
            }
        }
    }
    kill(getppid(), SIGTERM); // Terminate the police process when the game ends
}

void Police::catchGang(int gangID){
    std::cout << "Start arresting gang no." << gangID << std::endl;
    if (totalGangInfo[gangID] > 0) {
        ArrestMessage message = generate(gangID);
        if (msgsnd(msqID, &message, sizeof(ArrestMessage) - sizeof(long),0) == -1) {
            perror("msgsnd failed");
        }
        numberOfCaughtGangs++;
        std::cout << "Gang no." << gangID << " arrested for " << message.arrestPeriod << " seconds." << std::endl;
        std::cout << "Gang no." << gangID << "Total Gang info: " << totalGangInfo[gangID] << std::endl;
    }else{
        std::cout << "Arrest Failed!!!" << std::endl;
        std::cout << "Gang no." << gangID << "Total Gang info: " << totalGangInfo[gangID] << std::endl;
        numberOfSuccessfulOperations++;
    }   
}

ArrestMessage Police::generate(int gangID){
    int min = config.police.arrest_time_min;
    int max = config.police.arrest_time_max;
    int arrestPeriod = min + (rand() % (max - min + 1));
    ArrestMessage message;
    message.mtype = gangID; // Message type for message queue
    message.gangID = gangID;
    message.arrestPeriod = arrestPeriod;
    return message;
}
