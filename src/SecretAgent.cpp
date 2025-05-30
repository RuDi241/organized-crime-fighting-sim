#include "SecretAgent.h"
#include <sys/msg.h>
#include <iostream>

SecretAgent::SecretAgent(int ID, int rank, int trust, int policeMsqID) 
    : GangMember(ID, rank, trust), policeMsqID(policeMsqID) {
}

SecretAgent::SecretAgent(const GangMemberMessage &msg, int policeMsqID)
    : GangMember(msg), policeMsqID(policeMsqID) {
}

void SecretAgent::receiveInformation(InformationMessage message) {
    std::cout << "SecretAgent received message" << std::endl;
    infoList.push_back(message);
    sendToPolice(message);
}

void SecretAgent::sendToPolice(InformationMessage message) {
    AgentMessage agentMessage;
    agentMessage.mtype = message.mtype; // Message type for message queue
    if (message.type == InformationMessageType::NORMAL_INFO) {
        agentMessage.type = AgentMessageType::NORMAL_INFO;
    } else if (message.type == InformationMessageType::ATTACK) {
        agentMessage.type = AgentMessageType::ATTACK;
    } else {
        std::cerr << "Unknown information message type received." << std::endl;
        return; // Exit if the type is unknown
    }
    agentMessage.gangID = message.gangID;
    agentMessage.MessageID = message.MessageID;
    agentMessage.weight = message.weight;
    
    if (msgsnd(policeMsqID, &agentMessage, sizeof(AgentMessage) - sizeof(long), 0) == -1) {
        perror("SecretAgent: Failed to send message to police");
    }
}