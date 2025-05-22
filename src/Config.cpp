#include "Config.h"
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <memory>
#include "cJSON.h"

namespace {
// RAII wrapper for cJSON
class JsonPtr {
public:
  explicit JsonPtr(cJSON *json) : json_(json) {}
  ~JsonPtr() {
    if (json_)
      cJSON_Delete(json_);
  }

  JsonPtr(const JsonPtr &) = delete;
  JsonPtr &operator=(const JsonPtr &) = delete;

  JsonPtr(JsonPtr &&other) noexcept : json_(other.json_) {
    other.json_ = nullptr;
  }

  JsonPtr &operator=(JsonPtr &&other) noexcept {
    if (this != &other) {
      if (json_)
        cJSON_Delete(json_);
      json_ = other.json_;
      other.json_ = nullptr;
    }
    return *this;
  }

  cJSON *get() const { return json_; }
  cJSON *operator->() const { return json_; }
  explicit operator bool() const { return json_ != nullptr; }

private:
  cJSON *json_;
};

// Helper functions for JSON parsing
cJSON *get_object_item(cJSON *root, const char *key) {
  if (!root || !key)
    return nullptr;
  return cJSON_GetObjectItem(root, key);
}

int get_int_value(cJSON *obj, const char *key, int default_value = 0) {
  if (!obj || !key)
    return default_value;
  cJSON *item = cJSON_GetObjectItem(obj, key);
  return (item && cJSON_IsNumber(item)) ? item->valueint : default_value;
}

double get_double_value(cJSON *obj, const char *key,
                        double default_value = 0.0) {
  if (!obj || !key)
    return default_value;
  cJSON *item = cJSON_GetObjectItem(obj, key);
  return (item && cJSON_IsNumber(item)) ? item->valuedouble : default_value;
}

// Helper to add number to JSON object with error checking
void add_number_to_object(cJSON *obj, const char *key, double value) {
  if (!obj || !key) {
    throw std::runtime_error("Invalid parameters for JSON number addition");
  }
  if (!cJSON_AddNumberToObject(obj, key, value)) {
    throw std::runtime_error("Failed to add number to JSON object");
  }
}

// Helper to add object to JSON with error checking
void add_object_to_json(cJSON *parent, const char *key, cJSON *child) {
  if (!parent || !key || !child) {
    throw std::runtime_error("Invalid parameters for JSON object addition");
  }
  if (!cJSON_AddItemToObject(parent, key, child)) {
    throw std::runtime_error("Failed to add object to JSON");
  }
}
} // namespace

bool Config::load_from_file(const std::string &filename) {
  try {
    // Read file content
    std::ifstream file(filename);
    if (!file.is_open()) {
      return false;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    const std::string json_str = buffer.str();

    if (json_str.empty()) {
      return false;
    }

    // Parse JSON
    JsonPtr root(cJSON_Parse(json_str.c_str()));
    if (!root) {
      return false;
    }

    // Parse gang configuration
    if (cJSON *gang_json = get_object_item(root.get(), "gang")) {
      gang.num_gangs_min = get_int_value(gang_json, "num_gangs_min");
      gang.num_gangs_max = get_int_value(gang_json, "num_gangs_max");
      gang.num_members_min = get_int_value(gang_json, "num_members_min");
      gang.num_members_max = get_int_value(gang_json, "num_members_max");
      gang.num_ranks = get_int_value(gang_json, "num_ranks");
      gang.promotion_xp = get_int_value(gang_json, "promotion_xp");
      gang.p_death = get_double_value(gang_json, "p_death");
      gang.execution_threshold =
          get_int_value(gang_json, "execution_threshold");
    } else {
      return false;
    }

    // Parse target configuration
    if (cJSON *target_json = get_object_item(root.get(), "target")) {
      target.preparation_time_min =
          get_int_value(target_json, "preparation_time_min");
      target.preparation_time_max =
          get_int_value(target_json, "preparation_time_max");
      target.difficulty_min = get_int_value(target_json, "difficulty_min");
      target.difficulty_max = get_int_value(target_json, "difficulty_max");
      target.obscurity_min = get_int_value(target_json, "obscurity_min");
      target.obscurity_max = get_int_value(target_json, "obscurity_max");
      target.reward_min = get_int_value(target_json, "reward_min");
      target.reward_max = get_int_value(target_json, "reward_max");
      target.xp_min = get_int_value(target_json, "xp_min");
      target.xp_max = get_int_value(target_json, "xp_max");
    } else {
      return false;
    }

    // Parse info configuration
    if (cJSON *info_json = get_object_item(root.get(), "info")) {
      info.weight_min = get_int_value(info_json, "weight_min");
      info.weight_max = get_int_value(info_json, "weight_max");
      info.p_true = get_double_value(info_json, "p_true");
    } else {
      return false;
    }

    // Parse agent configuration
    if (cJSON *agent_json = get_object_item(root.get(), "agent")) {
      agent.initial_trust = get_int_value(agent_json, "initial_trust");
      agent.trust_decrement = get_int_value(agent_json, "trust_decrement");
      agent.inform_plans_threshold =
          get_int_value(agent_json, "inform_plans_threshold");
    } else {
      return false;
    }

    // Parse police configuration
    if (cJSON *police_json = get_object_item(root.get(), "police")) {
      police.start_threshold = get_int_value(police_json, "start_threshold");
      police.autostart_threshold =
          get_int_value(police_json, "autostart_threshold");
      police.arrest_time_min = get_int_value(police_json, "arrest_time_min");
      police.arrest_time_max = get_int_value(police_json, "arrest_time_max");
    } else {
      return false;
    }

    // Parse simulation end configuration
    if (cJSON *sim_end_json = get_object_item(root.get(), "sim_end")) {
      sim_end.gang_plans_thwarted =
          get_int_value(sim_end_json, "gang_plans_thwarted");
      sim_end.gang_plans_succeed =
          get_int_value(sim_end_json, "gang_plans_succeed");
      sim_end.executed_agents = get_int_value(sim_end_json, "executed_agents");
    } else {
      return false;
    }

    return true;

  } catch (const std::exception &) {
    return false;
  }
}

bool Config::dump_to_file(const std::string &filename) const {
  try {
    JsonPtr root(cJSON_CreateObject());
    if (!root) {
      return false;
    }

    // Create gang object
    cJSON *gang_obj = cJSON_CreateObject();
    if (!gang_obj)
      return false;

    add_number_to_object(gang_obj, "num_gangs_min", gang.num_gangs_min);
    add_number_to_object(gang_obj, "num_gangs_max", gang.num_gangs_max);
    add_number_to_object(gang_obj, "num_members_min", gang.num_members_min);
    add_number_to_object(gang_obj, "num_members_max", gang.num_members_max);
    add_number_to_object(gang_obj, "num_ranks", gang.num_ranks);
    add_number_to_object(gang_obj, "promotion_xp", gang.promotion_xp);
    add_number_to_object(gang_obj, "p_death", gang.p_death);
    add_number_to_object(gang_obj, "execution_threshold",
                         gang.execution_threshold);

    add_object_to_json(root.get(), "gang", gang_obj);

    // Create target object
    cJSON *target_obj = cJSON_CreateObject();
    if (!target_obj)
      return false;

    add_number_to_object(target_obj, "preparation_time_min",
                         target.preparation_time_min);
    add_number_to_object(target_obj, "preparation_time_max",
                         target.preparation_time_max);
    add_number_to_object(target_obj, "difficulty_min", target.difficulty_min);
    add_number_to_object(target_obj, "difficulty_max", target.difficulty_max);
    add_number_to_object(target_obj, "obscurity_min", target.obscurity_min);
    add_number_to_object(target_obj, "obscurity_max", target.obscurity_max);
    add_number_to_object(target_obj, "reward_min", target.reward_min);
    add_number_to_object(target_obj, "reward_max", target.reward_max);
    add_number_to_object(target_obj, "xp_min", target.xp_min);
    add_number_to_object(target_obj, "xp_max", target.xp_max);

    add_object_to_json(root.get(), "target", target_obj);

    // Create info object
    cJSON *info_obj = cJSON_CreateObject();
    if (!info_obj)
      return false;

    add_number_to_object(info_obj, "weight_min", info.weight_min);
    add_number_to_object(info_obj, "weight_max", info.weight_max);
    add_number_to_object(info_obj, "p_true", info.p_true);

    add_object_to_json(root.get(), "info", info_obj);

    // Create agent object
    cJSON *agent_obj = cJSON_CreateObject();
    if (!agent_obj)
      return false;

    add_number_to_object(agent_obj, "initial_trust", agent.initial_trust);
    add_number_to_object(agent_obj, "trust_decrement", agent.trust_decrement);
    add_number_to_object(agent_obj, "inform_plans_threshold",
                         agent.inform_plans_threshold);

    add_object_to_json(root.get(), "agent", agent_obj);

    // Create police object
    cJSON *police_obj = cJSON_CreateObject();
    if (!police_obj)
      return false;

    add_number_to_object(police_obj, "start_threshold", police.start_threshold);
    add_number_to_object(police_obj, "autostart_threshold",
                         police.autostart_threshold);
    add_number_to_object(police_obj, "arrest_time_min", police.arrest_time_min);
    add_number_to_object(police_obj, "arrest_time_max", police.arrest_time_max);

    add_object_to_json(root.get(), "police", police_obj);

    // Create sim_end object
    cJSON *sim_end_obj = cJSON_CreateObject();
    if (!sim_end_obj)
      return false;

    add_number_to_object(sim_end_obj, "gang_plans_thwarted",
                         sim_end.gang_plans_thwarted);
    add_number_to_object(sim_end_obj, "gang_plans_succeed",
                         sim_end.gang_plans_succeed);
    add_number_to_object(sim_end_obj, "executed_agents",
                         sim_end.executed_agents);

    add_object_to_json(root.get(), "sim_end", sim_end_obj);

    // Convert to string and write to file
    std::unique_ptr<char, decltype(&free)> json_str(cJSON_Print(root.get()),
                                                    &free);
    if (!json_str) {
      return false;
    }

    std::ofstream file(filename);
    if (!file.is_open()) {
      return false;
    }

    file << json_str.get();
    if (file.bad()) {
      return false;
    }

    return true;

  } catch (const std::exception &) {
    return false;
  }
}

Config::Config() {

  // Gang configuration with meaningful defaults
  gang = {.num_gangs_min = 1,
          .num_gangs_max = 4,
          .num_members_min = 5,
          .num_members_max = 10,
          .num_ranks = 4,
          .promotion_xp = 100,
          .p_death = 0.01,
          .execution_threshold = 50};

  // Target configuration
  target = {.preparation_time_min = 10,
            .preparation_time_max = 50,
            .difficulty_min = 1,
            .difficulty_max = 10,
            .obscurity_min = 1,
            .obscurity_max = 10,
            .reward_min = 100,
            .reward_max = 500,
            .xp_min = 10,
            .xp_max = 50};

  // Info configuration
  info = {.weight_min = 1, .weight_max = 5, .p_true = 0.8};

  // Agent configuration
  agent = {
      .initial_trust = 50, .trust_decrement = 1, .inform_plans_threshold = 20};

  // Police configuration
  police = {.start_threshold = 40,
            .autostart_threshold = 80,
            .arrest_time_min = 10,
            .arrest_time_max = 30};

  // Simulation end configuration
  sim_end = {.gang_plans_thwarted = 10,
             .gang_plans_succeed = 10,
             .executed_agents = 5};
}
