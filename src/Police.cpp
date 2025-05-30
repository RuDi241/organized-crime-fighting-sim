#include "Police.h"
#include <iostream>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>
#include "VisualizationMSQ.h"

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
            std::cout << "Police received message from gang no." << agentMessage.gangID << " with ID: " << agentMessage.MessageID << " with type: " << (int)agentMessage.type << std::endl;
            if (agentMessage.type == AgentMessageType::NORMAL_INFO) {
                std::cout << "Police received normal info from gang no." << agentMessage.gangID << " with ID: " << agentMessage.MessageID << std::endl;
                infoCounter[agentMessage.gangID][agentMessage.MessageID]++;

                if(infoCounter[agentMessage.gangID][agentMessage.MessageID] == 2){
                    std::cout << "Police received 2 info from gang no." << agentMessage.gangID << " with ID: " << agentMessage.MessageID << std::endl;
                    totalGangInfo[agentMessage.gangID] += agentMessage.weight;
                }           
                     
            } else if (agentMessage.type == AgentMessageType::ATTACK) {
                std::cout << "Police received attack message from gang no." << agentMessage.gangID << " with ID: " << agentMessage.MessageID << std::endl;
                catchGang(agentMessage.gangID);
                while(msgrcv(agentsMsqID, &agentMessage, sizeof(agentMessage)- sizeof(long), agentMessage.gangID, IPC_NOWAIT) > 0);
            } else {
                std::cout << "Unknown message type received from gang no." << agentMessage.gangID << " with ID: " << agentMessage.MessageID << std::endl;
            }

            VisualizationMessage vizMessage;
            vizMessage.mtype = MessageType::UPDATE_GANG;
            vizMessage.gangID = agentMessage.gangID;
            vizMessage.memberIdx = -1; // Not updating a specific member
            vizMessage.leaks = totalGangInfo[agentMessage.gangID];
            vizMessage.phase = -1; // Assuming phase 0 for simplicity
            vizMessage.capacity = -1; // Assuming no specific capacity update
            VisualizationMSQ::send(vizMessage);
        }
    }
    std::cout << "Police operation ended. Caught gangs: " << numberOfCaughtGangs << ", Successful operations: " << numberOfSuccessfulOperations << std::endl;
    std::cout << "Police process terminating..." << std::endl;
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
        std::cout << "OPERATION OF GANG NO." << gangID << " FAILD!!!" << std::endl;
        std::cout << "Gang no." << gangID << " arrested for " << message.arrestPeriod << " seconds." << std::endl;
        std::cout << "Gang no." << gangID << "Total Gang info: " << totalGangInfo[gangID] << std::endl;

        VisualizationMessage arrestMessage;
        arrestMessage.mtype = MessageType::UPDATE_GANG;
        arrestMessage.gangID = gangID;
        arrestMessage.memberIdx = -1; // Not updating a specific member
        arrestMessage.leaks = totalGangInfo[gangID];
        arrestMessage.phase = 2; // Assuming phase 0 for simplicity
        arrestMessage.capacity = -1; // Assuming no specific capacity update
        VisualizationMSQ::send(arrestMessage);
    }else{
        std::cout << "OPERATION OF GANG NO." << gangID << " SUCCESSED!!!" << std::endl;
        std::cout << "Arrest Failed!!!" << std::endl;
        std::cout << "Gang no." << gangID << "Total Gang info: " << totalGangInfo[gangID] << std::endl;
        numberOfSuccessfulOperations++;

        VisualizationMessage arrestMessage;
        arrestMessage.mtype = MessageType::UPDATE_GANG;
        arrestMessage.gangID = gangID;
        arrestMessage.memberIdx = -1; // Not updating a specific member
        arrestMessage.leaks = 0;
        arrestMessage.phase = -1; // Assuming phase 0 for simplicity
        arrestMessage.capacity = -1; // Assuming no specific capacity update
        VisualizationMSQ::send(arrestMessage);
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
