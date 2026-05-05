#ifndef COMMS_H
#define COMMS_H

#include <string>

struct CommsConfig {
    std::string node_id;
    double frequency_mhz;
    bool encryption_enabled;
    int max_message_length;
    std::string callsign;
    std::string network_id;
};

enum class CommsStatus {
    OK,
    INVALID_CONFIG,
    MESSAGE_TOO_LONG,
    EMPTY_MESSAGE,
    FREQUENCY_OUT_OF_RANGE
};

std::string status_to_string(CommsStatus status);

class CommsModule {
public:
    explicit CommsModule(const CommsConfig& config);

    CommsStatus validate_config() const;
    CommsStatus transmit_message(const std::string& message) const;
    const CommsConfig& get_config() const;

private:
    CommsConfig config_;

    static constexpr double MIN_FREQ_MHZ = 30.0;
    static constexpr double MAX_FREQ_MHZ = 6000.0;
};

#endif
