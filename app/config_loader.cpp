#include "config_loader.h"
#include <yaml-cpp/yaml.h>
#include <stdexcept>

CommsConfig load_config(const std::string& filepath) {
    YAML::Node root;
    try {
        root = YAML::LoadFile(filepath);
    } catch (const YAML::Exception& e) {
        throw std::runtime_error("Failed to load config: " + std::string(e.what()));
    }

    CommsConfig config;

    if (!root["node_id"]) {
        throw std::runtime_error("Missing required field: node_id");
    }
    config.node_id = root["node_id"].as<std::string>();

    if (!root["frequency_mhz"]) {
        throw std::runtime_error("Missing required field: frequency_mhz");
    }
    config.frequency_mhz = root["frequency_mhz"].as<double>();

    config.encryption_enabled = root["encryption_enabled"]
        ? root["encryption_enabled"].as<bool>()
        : false;

    config.max_message_length = root["max_message_length"]
        ? root["max_message_length"].as<int>()
        : 256;

    config.callsign = root["callsign"]
        ? root["callsign"].as<std::string>()
        : "UNKNOWN";

    config.network_id = root["network_id"]
        ? root["network_id"].as<std::string>()
        : "DEFAULT";

    return config;
}
