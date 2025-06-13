#include "VisualizationMSQ.h"
#include <iostream>
#define GANG_COLOR 0.0f, 0.0f, 1.0f // Bright Blue for gang lines, inset
#define STAR_COLOR 1.0f, 0.5f, 0.0f // Orange for secret agent star
#define TEXT_COLOR                                                             \
  1.0f, 1.0f, 1.0f // White for labels, titles, leaks, status bar text
#define STATUS_BAR_COLOR 0.3f, 0.3f, 0.3f // Dark Gray for status bar background
#define BACKGROUND_COLOR 0.2f, 0.5f, 0.2f // Greenish for background
#define CIRCLE_COLOR                                                           \
  0.7f, 0.7f, 0.7f // Light Gray for member circle base and border

// Column positions for gang members
#define COLUMN_X_START -0.95f  // X position of first gang column
#define COLUMN_X_SPACING 0.25f // Spacing between gang columns

// Member positions and visuals
#define MEMBER_Y_START 0.5f          // Starting y for first member
#define MEMBER_Y_SPACING 0.3f        // Vertical spacing between members
#define MEMBER_LINE_LENGTH 0.125f    // Length of horizontal line to member
#define MEMBER_BASE_RADIUS 0.075f    // Radius of base circle
#define MEMBER_INSET_RADIUS 0.05f    // Radius of inset circle
#define MEMBER_BORDER_RADIUS 0.1f    // Radius of border circle
#define MEMBER_LABEL_SCALE 0.4f      // Text scale for member labels
#define MEMBER_LABEL_Y_OFFSET -0.16f // Y offset above member for label

// Triangle: Star (secret agent)
#define STAR_SIZE 0.05f     // Side length of secret agent star triangle
#define STAR_Y_OFFSET -0.1f // Y offset below member circle

// Gang titles
#define TITLE_X_START -0.9f   // X position of first gang title
#define TITLE_X_SPACING 0.25f // Spacing between gang titles
#define TITLE_Y 0.85f         // Y position of gang titles
#define TITLE_SCALE 0.5f      // Text scale for gang titles

// Phase circle
#define PHASE_CIRCLE_X_OFFSET 0.125f // X offset from column for phase circle
#define PHASE_CIRCLE_Y 0.1f          // Y position of phase circle
#define PHASE_CIRCLE_RADIUS 0.04f    // Radius of phase circle

// Leaks text
#define LEAKS_TEXT_X_OFFSET 0.125f // X offset from column for leaks text
#define LEAKS_TEXT_Y 0.68f         // Y position of leaks text
#define LEAKS_TEXT_SCALE 0.5f      // Text scale for leaks text

// Status bar
#define STATUS_BAR_Y_TOP 1.0f           // Top y of status bar
#define STATUS_BAR_Y_BOTTOM 0.6f        // Bottom y of status bar
#define STATUS_BAR_TEXT_X_START -0.9f   // X position of first status bar text
#define STATUS_BAR_TEXT_X_SPACING 0.25f // Spacing between status bar texts
#define STATUS_BAR_TEXT_Y 0.75f         // Y position of status bar text
#define STATUS_BAR_TEXT_SCALE 0.5f      // Text scale for status bar text

constexpr float STAR_HEIGHT = STAR_SIZE * 0.8660254f; // sqrt(3)/2

#include "Graphics.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GangStruct.h"
#include "MemberGeneratorMessage.h"
#include "renderer.h"

static int window_width = 1280;
static int window_height = 720;

#define TARGET_FRAME_DURATION 0.0069

Graphics::Graphics() {}
void Graphics::run() {
  if (!glfwInit()) {
    printf("Failed to initialize GLFW\n");
    return;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  GLFWwindow *window =
      glfwCreateWindow(window_width, window_height, "Bakery", nullptr, nullptr);
  if (!window) {
    printf("Failed to create GLFW window\n");
    glfwTerminate();
    return;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    printf("Failed to initialize GLAD\n");
    glfwDestroyWindow(window);
    glfwTerminate();
    return;
  }
  setWindowSize(window_width, window_height);

  initRenderer(window_width, window_height);

  printf("Loading font...\n");
  initTextRenderer("fonts/arial.ttf", 64);
  printf("Font loaded successfully\n");

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  Update();

  double startTime = glfwGetTime();

  while (!glfwWindowShouldClose(window)) {
    double frameStart = glfwGetTime();
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    setWindowSize(width, height);

    glClearColor(BACKGROUND_COLOR, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    Update();
    drawScene(frameStart - startTime);

    glfwSwapBuffers(window);
    glfwPollEvents();

    double elapsed = glfwGetTime() - frameStart;
    double sleepTime = TARGET_FRAME_DURATION - elapsed;
    if (sleepTime > 0) {
      struct timespec ts;
      ts.tv_sec = (time_t)floor(sleepTime);
      ts.tv_nsec = (long)((sleepTime - ts.tv_sec) * 1e9);
      nanosleep(&ts, nullptr);
    }
  }

  shutdownTextRenderer();
  shutdownRenderer();
  glfwDestroyWindow(window);
  glfwTerminate();
}

void Graphics::generateExampleData() {
  gangs.clear();
  gangs.resize(5); // Ensure space for gang IDs 1 to 4, index 0 unused

  GangStruct gang1;
  gang1.ID = 1;
  gang1.capacity = 10;
  gang1.phase = GangPhase::PREPARATION;
  gang1.leaks = 0;
  gang1.GangMembers = {{1, 3, 4, 5, false, GangMemberType::GANG_MEMBER},
                       {2, 1, 1, 0, false, GangMemberType::SECRET_AGENT},
                       {3, 1, 3, 10, true, GangMemberType::GANG_MEMBER},
                       {4, 2, 2, 3, false, GangMemberType::GANG_MEMBER}};
  gangs[1] = gang1;

  GangStruct gang2;
  gang2.ID = 2;
  gang2.capacity = 10;
  gang2.phase = GangPhase::OPERATION;
  gang2.leaks = 1;
  gang2.GangMembers = {{5, 4, 5, 8, true, GangMemberType::GANG_MEMBER},
                       {6, 2, 3, 6, false, GangMemberType::SECRET_AGENT},
                       {7, 3, 2, 4, false, GangMemberType::GANG_MEMBER},
                       {8, 1, 1, 0, false, GangMemberType::GANG_MEMBER},
                       {9, 5, 4, 10, true, GangMemberType::GANG_MEMBER},
                       {10, 2, 3, 2, false, GangMemberType::GANG_MEMBER}};
  gangs[2] = gang2;

  GangStruct gang3;
  gang3.ID = 3;
  gang3.capacity = 5;
  gang3.phase = GangPhase::JAILED;
  gang3.leaks = 2;
  gang3.GangMembers = {{11, 3, 3, 7, true, GangMemberType::GANG_MEMBER},
                       {12, 2, 2, 5, false, GangMemberType::SECRET_AGENT},
                       {13, 4, 4, 9, true, GangMemberType::GANG_MEMBER}};
  gangs[3] = gang3;

  GangStruct gang4;
  gang4.ID = 4;
  gang4.capacity = 10;
  gang4.phase = GangPhase::PREPARATION;
  gang4.leaks = 0;
  gang4.GangMembers = {{14, 1, 1, 2, false, GangMemberType::GANG_MEMBER},
                       {15, 3, 3, 4, false, GangMemberType::GANG_MEMBER},
                       {16, 5, 5, 10, true, GangMemberType::SECRET_AGENT},
                       {17, 2, 2, 3, false, GangMemberType::GANG_MEMBER},
                       {18, 4, 4, 6, false, GangMemberType::GANG_MEMBER}};
  gangs[4] = gang4;
}

void Graphics::drawStatusBar(double currentTime) {
  float opacity = std::min(1.0f, (float)currentTime / 1.0f);
  Rectangle rect = {-1.0f,
                    STATUS_BAR_Y_BOTTOM,
                    2.0f,
                    STATUS_BAR_Y_TOP - STATUS_BAR_Y_BOTTOM,
                    {STATUS_BAR_COLOR, opacity}};
  drawRectangle(rect);

  float x = STATUS_BAR_TEXT_X_START;
  for (size_t i = 1; i < gangs.size(); ++i) {
    if (gangs[i].ID == 0)
      continue; // Skip unused gangs
    std::string phaseText;
    switch (gangs[i].phase) {
    case GangPhase::PREPARATION:
      phaseText = "Prep";
      break;
    case GangPhase::OPERATION:
      phaseText = "Op";
      break;
    case GangPhase::JAILED:
      phaseText = "Jailed";
      break;
    }
    std::string text = "Gang " + std::to_string(gangs[i].ID) + ": " + phaseText;
    renderTextLeft(text.c_str(), x, STATUS_BAR_TEXT_Y, STATUS_BAR_TEXT_SCALE,
                   {TEXT_COLOR, opacity});
    x += STATUS_BAR_TEXT_X_SPACING;
  }
}

void Graphics::drawGang(const GangStruct &gang, size_t gangIndex,
                        double currentTime) {
  float opacity = 1.0f;
  Color gangColor = {GANG_COLOR, opacity};
  float columnX = COLUMN_X_START + (gangIndex - 1) * COLUMN_X_SPACING;

  // Draw members
  for (size_t i = 0; i < gang.GangMembers.size(); ++i) {
    float memberY = MEMBER_Y_START - i * MEMBER_Y_SPACING;
    Triangle memberLine = {
        columnX, memberY,  columnX, memberY, columnX + MEMBER_LINE_LENGTH,
        memberY, gangColor};
    drawTriangle(memberLine);
    drawMember(gang.GangMembers[i], columnX + MEMBER_LINE_LENGTH, memberY,
               opacity);
  }

  // Draw leaks text
  std::string leakText = "Leaks: " + std::to_string(gang.leaks);
  renderTextCenter(leakText.c_str(), columnX + LEAKS_TEXT_X_OFFSET,
                   LEAKS_TEXT_Y, LEAKS_TEXT_SCALE, {TEXT_COLOR, opacity});

  // Draw gang title
  float titleX = TITLE_X_START + (gangIndex - 1) * TITLE_X_SPACING;
  std::string gangText = "Gang " + std::to_string(gang.ID) + " (" +
                         std::to_string(gang.GangMembers.size()) + "/" +
                         std::to_string(gang.capacity) + ")";
  renderTextLeft(gangText.c_str(), titleX, TITLE_Y, TITLE_SCALE,
                 {TEXT_COLOR, opacity});
}

void Graphics::drawMember(const MemberStruct &member, float x, float y,
                          float opacity) {
  Color gangColor = {GANG_COLOR, opacity};

  // Draw base circle
  Circle baseCircle = {x, y, MEMBER_BASE_RADIUS, {CIRCLE_COLOR, opacity}, 20};
  drawCircle(baseCircle);

  // Draw inset circle
  Circle insetCircle = {x, y, MEMBER_INSET_RADIUS, gangColor, 20};
  drawCircle(insetCircle);

  // Draw border circle
  Circle borderCircle = {
      x, y, MEMBER_BORDER_RADIUS, {CIRCLE_COLOR, opacity}, 0};
  drawCircle(borderCircle);

  // Draw secret agent star (orange equilateral triangle pointing down if
  // SECRET_AGENT)
  if (member.type == GangMemberType::SECRET_AGENT) {
    float cx = x + MEMBER_INSET_RADIUS;
    float cy = y - MEMBER_BASE_RADIUS - STAR_Y_OFFSET;
    Triangle star = {cx,
                     cy - STAR_HEIGHT / 2.0f, // Bottom vertex
                     cx - STAR_SIZE / 2.0f,
                     cy + STAR_HEIGHT / 2.0f, // Top left
                     cx + STAR_SIZE / 2.0f,
                     cy + STAR_HEIGHT / 2.0f, // Top right
                     {STAR_COLOR, opacity}};
    drawTriangle(star);
  }

  // Draw member label
  std::string label = "M" + std::to_string(member.ID) + ": R" +
                      std::to_string(member.rank) + ", T" +
                      std::to_string(member.trust) + ", P" +
                      std::to_string(member.preparation_counter);
  renderTextCenter(label.c_str(), x, y + MEMBER_LABEL_Y_OFFSET,
                   MEMBER_LABEL_SCALE, {TEXT_COLOR, opacity});
}

void Graphics::drawScene(double currentTime) {
  drawStatusBar(currentTime);
  for (size_t i = 1; i < gangs.size(); ++i) {
    drawGang(gangs[i], i, currentTime);
  }
}

void Graphics::Update() {
  VisualizationMessage msg;
  // Process all available messages in the queue
  while (VisualizationMSQ::try_receive(msg)) {
    std::cout << VisualizationMSQ::msqid << std::endl;
    std::cout << "Received message:\n";
    std::cout << "  gangID: " << msg.gangID << "\n";
    std::cout << "  memberIdx: " << msg.memberIdx << "\n";
    std::cout << "  leaks: " << msg.leaks << "\n";
    std::cout << "  phase: " << msg.phase << "\n";
    std::cout << "  capacity: " << msg.capacity << "\n";
    std::cout << "  member.ID: " << msg.member.ID << "\n";
    std::cout << "  member.rank: " << msg.member.rank << "\n";
    std::cout << "  member.trust: " << msg.member.trust << "\n";
    std::cout << "  member.preparation_counter: "
              << msg.member.preparation_counter << "\n";
    std::cout << "  member.ready: " << (msg.member.ready ? "true" : "false")
              << "\n";
    std::cout << "  member.type: " << msg.member.type
              << "\n"; // prints enum as int
    switch (static_cast<MessageType>(msg.mtype)) {
    case ADD_GANG: {
      // Don't add if gangID already exists
      if (msg.gangID >= 1 && msg.gangID >= static_cast<int>(gangs.size())) {
        gangs.resize(msg.gangID + 1); // Ensure enough space
      }
      GangStruct &newGang = gangs[msg.gangID];
      newGang.ID = msg.gangID;
      if (msg.leaks != -1)
        newGang.leaks = msg.leaks;
      if (msg.capacity != -1)
        newGang.capacity = msg.capacity;
      if (msg.phase != -1)
        newGang.phase = static_cast<GangPhase>(msg.phase);
      break;
    }

    case REMOVE_GANG: {
      if (msg.gangID >= 1 && msg.gangID < static_cast<int>(gangs.size())) {
        gangs[msg.gangID].GangMembers.clear();
        gangs[msg.gangID] = GangStruct(); // Reset the gang
      }
      break;
    }

    case UPDATE_GANG: {
      if (msg.gangID >= 1 && msg.gangID < static_cast<int>(gangs.size())) {
        GangStruct &gang = gangs[msg.gangID];
        if (msg.leaks != -1)
          gang.leaks = msg.leaks;
        if (msg.phase != -1)
          gang.phase = static_cast<GangPhase>(msg.phase);
        if (msg.capacity != -1)
          gang.capacity = msg.capacity;
      }
      break;
    }

    case ADD_MEMBER: {
      if (msg.gangID >= 1 && msg.gangID < static_cast<int>(gangs.size())) {
        GangStruct &gang = gangs[msg.gangID];
        gang.GangMembers.push_back(msg.member);
      }
      break;
    }

    case REMOVE_MEMBER: {
      if (msg.gangID >= 1 && msg.gangID < static_cast<int>(gangs.size())) {
        GangStruct &gang = gangs[msg.gangID];
        if (msg.memberIdx >= 0 &&
            msg.memberIdx < static_cast<int>(gang.GangMembers.size())) {
          gang.GangMembers.erase(gang.GangMembers.begin() + msg.memberIdx);
        }
      }
      break;
    }

    case UPDATE_MEMBER: {
      if (msg.gangID >= 1 && msg.gangID < static_cast<int>(gangs.size())) {
        GangStruct &gang = gangs[msg.gangID];
        if (msg.memberIdx >= 0 &&
            msg.memberIdx < static_cast<int>(gang.GangMembers.size())) {
          MemberStruct &member = gang.GangMembers[msg.memberIdx];

          if (msg.member.ID != -1)
            member.ID = msg.member.ID;
          if (msg.member.rank != -1)
            member.rank = msg.member.rank;
          if (msg.member.trust != -1)
            member.trust = msg.member.trust;
          if (msg.member.preparation_counter != -1)
            member.preparation_counter = msg.member.preparation_counter;
          if (static_cast<int>(msg.member.ready) != -1)
            member.ready = msg.member.ready;
          if (static_cast<int>(msg.member.type) != -1)
            member.type = msg.member.type;
        }
      }
      break;
    }

    default:
      break;
    }
  }
}
