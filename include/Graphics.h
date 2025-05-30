#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "GangStruct.h"

class Graphics {
private:
  std::vector<GangStruct> gangs;
  void drawStatusBar(double currentTime);
  void drawGang(const GangStruct &gang, size_t gangIndex, double currentTime);
  void drawMember(const MemberStruct &member, float x, float y, float opacity);
  void generateExampleData();

public:
  Graphics();
  void run();
  void drawScene(double currentTime);
  void Update();
};

;

#endif
