#ifndef POLICE_H
#define POLICE_H
#include <map>
#include <vector>
#include "Config.h"
#include "AgentToPoliceMessage.h"
#include "ArrestMessage.h"

class Police {
    public:
        Police() = delete;
        Police(const Config &config, int msq_id, int receive_msq_id);
        void run();  
        int getNumberOfCaughtGangs();
        int getNumberOfSuccessfulOperations();
    private:
        const Config &config;
        int msqID;
        int agentsMsqID;
        AgentMessage agentMessage;
        std::map<int, std::map<int,int>> infoCounter;
        std::map<int, int> totalGangInfo;
        void catchGang(int gangID);
        ArrestMessage generate(int gangID);
        int numberOfCaughtGangs = 0;
        int numberOfSuccessfulOperations = 0;
};

#endif
