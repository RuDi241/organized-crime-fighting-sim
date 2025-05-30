#ifndef GRAPHICS_H
#define GRAPHICS_H
#include "GangStruct.h"
#include "VisualizationMSQ.h"
#include "renderer.h"

class Graphics {
private:
  std::vector<GangStruct> gangs;
  /*
  struct AnimationState {
    float opacity;
    double startTime;
  };
  std::vector<std::vector<AnimationState>> memberAnimations;
  std::vector<AnimationState> gangAnimations;
  double currentTime;
  */

  // Colors for gangs (up to 5)
  const Color gangColors[5] = {
      {1.0f, 0.4f, 0.4f, 1.0f}, // Red
      {0.4f, 0.4f, 1.0f, 1.0f}, // Blue
      {0.4f, 1.0f, 0.4f, 1.0f}, // Green
      {0.7f, 0.4f, 1.0f, 1.0f}, // Purple
      {1.0f, 0.7f, 0.4f, 1.0f}  // Orange
  };

  // Colors for phases
  const Color phaseColors[3] = {
      {1.0f, 1.0f, 0.0f, 1.0f}, // Yellow (Preparation)
      {1.0f, 0.5f, 0.0f, 1.0f}, // Orange (Operation)
      {0.5f, 0.5f, 0.5f, 1.0f}  // Gray (Jailed)
  };

  void drawStatusBar(double currentTime);
  void drawGang(const GangStruct &gang, size_t gangIndex, double currentTime);
  void drawMember(const MemberStruct &member, float x, float y, float opacity);
  void generateExampleData();

public:
  Graphics() {}
  void run();
  void drawScene(double currentTime);
  // void update(const VisualizationMessage& msg, double currentTime);
};

;

#endif
