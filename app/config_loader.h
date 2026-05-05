#ifndef CONFIG_LOADER_H
#define CONFIG_LOADER_H

#include "comms.h"
#include <string>

CommsConfig load_config(const std::string& filepath);

#endif
