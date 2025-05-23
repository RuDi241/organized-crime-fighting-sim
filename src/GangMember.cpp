#include "GangMember.h"
#include "MemberGenerator.h"
// Constructor
GangMember::GangMember(int ID, int rank, int trust)
    : ID(ID), rank(rank), trust(trust), preparation_counter(0), ready(false) {}

GangMember::GangMember(const GangMemberMessage &msg)
    : ID(msg.ID), rank(msg.rank), trust(msg.trust) {}

// Getter for ID
int GangMember::getID() const { return ID; }

// Getter for rank
int GangMember::getRank() const { return rank; }

// Setter for rank
void GangMember::setRank(int newRank) { rank = newRank; }

// Simulates some preparation logic
void GangMember::prepare() {
  preparation_counter++;
  if (preparation_counter >= 3) {
    ready = true;
  }
}

// Check if ready
bool GangMember::isReady() const { return ready; }
