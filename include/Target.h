#ifndef TARGET_H
#define TARGET_H

#include <vector>
#include <string>

enum class OperationType {
  RobbingBanks,
  StealingJewelry,
  DrugTrafficking,
  StealingArt,
  Kidnapping,
  BlackMailing,
  ArmsTrafficking,
  COUNT,
};

// String conversion (to string)
inline const char *operation_type_to_string(OperationType op) {
  switch (op) {
  case OperationType::RobbingBanks:
    return "Robbing Bank";
  case OperationType::StealingJewelry:
    return "Stealing Jewelry";
  case OperationType::DrugTrafficking:
    return "Drug Trafficking";
  case OperationType::StealingArt:
    return "Stealing Art";
  case OperationType::Kidnapping:
    return "Kidnapping";
  case OperationType::BlackMailing:
    return "Black Mailing";
  case OperationType::ArmsTrafficking:
    return "Arms Trafficking";
  default:
    return "Unknown Operation";
  }
}

struct Target {
  enum OperationType value;
  int id;
  int required_preparation;
  int difficulty;
  int obscurity;
  int reward;
  int xp;
};

struct TargetMessage {
  long mType;
  Target target;
};
#endif
