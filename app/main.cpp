#include "comms.h"
#include "config_loader.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char* argv[]) {
    std::string config_path = "config/comms.yaml";
    if (argc > 1) {
        config_path = argv[1];
    }

    std::cout << "=== Tactical Comms Simulator ===" << std::endl;
    std::cout << "Loading config: " << config_path << std::endl;

    CommsConfig config;
    try {
        config = load_config(config_path);
    } catch (const std::exception& e) {
        std::cerr << "CONFIG ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    CommsModule comms(config);

    CommsStatus status = comms.validate_config();
    if (status != CommsStatus::OK) {
        std::cerr << "VALIDATION FAILED: " << status_to_string(status) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Node " << config.node_id
              << " validated on " << config.frequency_mhz << " MHz" << std::endl;
    std::cout << "Callsign: " << config.callsign
              << " | Network: " << config.network_id << std::endl;

    status = comms.transmit_message("COMMS CHECK - SYSTEM OPERATIONAL");
    if (status != CommsStatus::OK) {
        std::cerr << "TRANSMIT FAILED: " << status_to_string(status) << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Transmission complete." << std::endl;
    return EXIT_SUCCESS;
}
