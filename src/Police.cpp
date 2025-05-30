#include "Police.h"
#include <iostream>
#include <sys/msg.h>
#include <unistd.h>


Police::Police(const Config &config, int msq_id, int receive_msq_id)
    : config(config), msqID(msq_id), agentsMsqID(receive_msq_id){
  srand(time(NULL));
}

void Police::run() {
    while (true)
    {
        ssize_t agentMessageSize = msgrcv(agentsMsqID, &agentMessage, sizeof(agentMessage)- sizeof(long),0, IPC_NOWAIT); 
        if (agentMessageSize == -1) {
            perror("msgrcv failed.");
        } else if (agentMessageSize > 0) {
            if (agentMessage.type == AgentMessageType::NORMAL_INFO) {
                if (infoCounter.find(agentMessage.gangID) == NULL || infoCounter[agentMessage.gangID].find(agentMessage.MessageID) == infoCounter[agentMessage.gangID].end()) {
                    infoCounter[agentMessage.gangID][agentMessage.MessageID] = 1;
                } else {
                    infoCounter[agentMessage.gangID][agentMessage.MessageID]++;
                    if(infoCounter[agentMessage.gangID][agentMessage.MessageID] == 2){
                        totalGangInfo[agentMessage.gangID] += agentMessage.weight;
                    }
                }                
            } else if (agentMessage.type == AgentMessageType::ATTACK) {
                catchGang(agentMessage.gangID);
            } else {
                sleep(1);
            }
        }
    }
}

void Police::catchGang(int gangID){
    std::cout << "Start arresting gang no." << gangID << std::endl;
    if (totalGangInfo[gangID] > 0) {
        ArrestMessage message = generate(gangID);
        if (msgsnd(msqID, &message, sizeof(ArrestMessage) - sizeof(long),0) == -1) {
            perror("msgsnd failed");
        }
    }else{
        std::cout << "Arrest Failed!!!" << std::endl;
    }   
}

ArrestMessage Police::generate(int gangID){
    int min = config.police.arrest_time_min;
    int max = config.police.arrest_time_max;
    int arrestPeriod = min + (rand() % (max - min + 1));
    ArrestMessage message;
    
    message.gangID = gangID;
    message.arrestPeriod = arrestPeriod;
    return message;
}
