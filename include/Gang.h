#ifndef GANG_H
#define GANG_H

#include <vector>
#include "GangMember.h"

class Gang {
public:
  std::vector<GangMember> GangMembers;
  //std::vector<Target> targets;

  Gang(int ID, int capacity, int acceptacne_rate);
  Gang() = delete;

  int getID() const;
  int getCapacity() const;
  int getAcceptanceRate() const;
  void acceptMember(GangMember GangMember);
  void informGangMembers();
  void startOperation();
  void leaveJail();
  void run();

private:
  int ID;
  int capacity;
  int acceptance_rate;
};

#endif
