# Shared library of comms logic (used by both the app and tests)
add_library(comms_lib STATIC
    comms.cpp
    config_loader.cpp
)

target_include_directories(comms_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(comms_lib PUBLIC yaml-cpp::yaml-cpp)

# Main executable
add_executable(tactical-comms-sim main.cpp)
target_link_libraries(tactical-comms-sim PRIVATE comms_lib)
