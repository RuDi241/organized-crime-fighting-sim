#ifndef MEMBER_H
#define MEMBER_H

#include <string>

class Member {
public:
  Member(const std::string &name, int ID, int rank, int trust);
  Member() = delete;

  const std::string &getName() const;
  int getID() const;
  int getRank() const;
  void setRank(int rank);
  void prepare();
  bool isReady() const;
  bool isTrustable() const;
  // virtual void acceptInformation(const Information &info) = delete;

private:
  int ID;
  int rank;
  std::string name;
  int preparation_counter;
  int trust;
};

#endif
