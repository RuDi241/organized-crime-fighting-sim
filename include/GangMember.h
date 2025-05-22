#ifndef GANG_MEMBER_H
#define GANG_MEMBER_H


class GangMember {
  public:
      GangMember(int ID, int rank, int trust);
      GangMember() = delete;  // Disable default constructor
  
      int getID() const;
      int getRank() const;
      void setRank(int rank);
      void prepare();
      bool isReady() const;
  
  private:
      int ID;
      int rank;
      int trust;
      int preparation_counter = 0;
      bool ready = false;  // Replaces int isReady for clarity
  };

#endif
