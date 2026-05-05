#include "comms.h"
#include <iostream>

std::string status_to_string(CommsStatus status) {
    switch (status) {
        case CommsStatus::OK:                   return "OK";
        case CommsStatus::INVALID_CONFIG:        return "INVALID_CONFIG";
        case CommsStatus::MESSAGE_TOO_LONG:      return "MESSAGE_TOO_LONG";
        case CommsStatus::EMPTY_MESSAGE:         return "EMPTY_MESSAGE";
        case CommsStatus::FREQUENCY_OUT_OF_RANGE: return "FREQUENCY_OUT_OF_RANGE";
    }
    return "UNKNOWN";
}

CommsModule::CommsModule(const CommsConfig& config)
    : config_(config) {}

CommsStatus CommsModule::validate_config() const {
    if (config_.node_id.empty()) {
        return CommsStatus::INVALID_CONFIG;
    }
    if (config_.frequency_mhz < MIN_FREQ_MHZ ||
        config_.frequency_mhz > MAX_FREQ_MHZ) {
        return CommsStatus::FREQUENCY_OUT_OF_RANGE;
    }
    if (config_.max_message_length <= 0) {
        return CommsStatus::INVALID_CONFIG;
    }
    return CommsStatus::OK;
}

CommsStatus CommsModule::transmit_message(const std::string& message) const {
    if (message.empty()) {
        return CommsStatus::EMPTY_MESSAGE;
    }
    if (static_cast<int>(message.length()) > config_.max_message_length) {
        return CommsStatus::MESSAGE_TOO_LONG;
    }

    std::cout << "[" << config_.callsign << "@" << config_.network_id << "] "
              << "TX on " << config_.frequency_mhz << " MHz"
              << (config_.encryption_enabled ? " [ENCRYPTED]" : " [PLAINTEXT]")
              << ": " << message << std::endl;

    return CommsStatus::OK;
}

const CommsConfig& CommsModule::get_config() const {
    return config_;
}
