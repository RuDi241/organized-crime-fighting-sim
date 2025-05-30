#ifndef GANG_H
#define GANG_H

#include <vector>
#include "GangMember.h"
#include "Config.h"
#include "utils.h"

class Gang {
public:
  std::vector<GangMember> GangMembers;
  //std::vector<Target> targets;

  Gang(const Config &config, int ID, int capacity, int acceptacne_rate, int member_generator_msq_id);
  Gang() = delete;

  int getID() const;
  int getCapacity() const;
  int getAcceptanceRate() const;
  void acceptMember();
  void informGangMembers();
  void startOperation();
  void leaveJail();
  void run();

private:
  int ID;
  int capacity;
  int acceptance_rate;
  const Config &config;
  int messageIdGenerator = 1;
  int memberGeneratorMsqID;
};

#endif
