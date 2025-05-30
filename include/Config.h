#ifndef CONFIG_H
#define CONFIG_H

#include <string>

struct GangConfig {
  int num_gangs_min;
  int num_gangs_max;
  int num_members_min;
  int num_members_max;
  int num_ranks;
  // The xp required for promotion is (num_ranks-rank)*promotion_xp
  int promotion_xp;
  double p_death;
  int execution_threshold;
  int member_generation_delay;
};

struct TargetConfig {
  // The time required to prepare for the operation
  int preparation_time_min;
  int preparation_time_max;
  // Part of target selection algorithm, affects the probability of success;
  int difficulty_min;
  int difficulty_max;
  // Determines if a gang sees this target
  int obscurity_min;
  int obscurity_max;
  // Money rewarded to the gang
  int reward_min;
  int reward_max;
  // xp rewarded to each member
  int xp_min;
  int xp_max;
  int target_generation_delay;
};

struct InfoConfig {
  // The value of the information
  int weight_min;
  int weight_max;
  // Probability of generated info being true
  double p_true;
  double p_spread;
};

struct AgentConfig {
  int initial_trust;
  int trust_decrement;
  int inform_plans_threshold;
  // probability of new member being agent
  double p_agent;
};

struct PoliceConfig {
  int start_threshold;
  int autostart_threshold;
  int arrest_time_min;
  int arrest_time_max;
};

struct SimEndConfig {
  int gang_plans_thwarted;
  int gang_plans_succeed;
  int executed_agents;
};

struct Config {
  GangConfig gang;
  TargetConfig target;
  InfoConfig info;
  AgentConfig agent;
  PoliceConfig police;
  SimEndConfig sim_end;
  bool load_from_file(const std::string &filename);
  bool dump_to_file(const std::string &filename) const;
  Config();
};
#endif
