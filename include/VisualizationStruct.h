#ifndef VISUALIZATION_STRUCT
#define VISUALIZATION_STRUCT

#include "InformationMessage.h"
#include "MemberGeneratorMessage.h"
#include <vector>

struct VisualizationStruct {
  std::vector<GangStruct> gangs;
  std::vector<int> totalGangInfo;
};
#endif
