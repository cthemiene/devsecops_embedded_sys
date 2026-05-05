#include <gtest/gtest.h>
#include "config_loader.h"
#include <fstream>
#include <cstdlib>
#include <filesystem>

namespace fs = std::filesystem;

class ConfigLoaderTest : public ::testing::Test {
protected:
    fs::path temp_dir;

    void SetUp() override {
        temp_dir = fs::temp_directory_path() / "comms_test";
        fs::create_directories(temp_dir);
    }

    void TearDown() override {
        fs::remove_all(temp_dir);
    }

    std::string write_yaml(const std::string& filename,
                           const std::string& content) {
        auto path = temp_dir / filename;
        std::ofstream out(path);
        out << content;
        out.close();
        return path.string();
    }
};

TEST_F(ConfigLoaderTest, LoadsValidConfigCorrectly) {
    auto path = write_yaml("valid.yaml",
        "node_id: \"BRAVO-3\"\n"
        "frequency_mhz: 300.0\n"
        "encryption_enabled: false\n"
        "max_message_length: 128\n"
        "callsign: \"HAWK\"\n"
        "network_id: \"SKYNET-1\"\n"
    );

    auto cfg = load_config(path);
    EXPECT_EQ(cfg.node_id, "BRAVO-3");
    EXPECT_DOUBLE_EQ(cfg.frequency_mhz, 300.0);
    EXPECT_FALSE(cfg.encryption_enabled);
    EXPECT_EQ(cfg.max_message_length, 128);
    EXPECT_EQ(cfg.callsign, "HAWK");
    EXPECT_EQ(cfg.network_id, "SKYNET-1");
}

TEST_F(ConfigLoaderTest, MissingNodeIdThrows) {
    auto path = write_yaml("no_node.yaml",
        "frequency_mhz: 300.0\n"
    );

    EXPECT_THROW(load_config(path), std::runtime_error);
}

TEST_F(ConfigLoaderTest, MissingFrequencyThrows) {
    auto path = write_yaml("no_freq.yaml",
        "node_id: \"TEST\"\n"
    );

    EXPECT_THROW(load_config(path), std::runtime_error);
}

TEST_F(ConfigLoaderTest, MissingOptionalFieldsGetDefaults) {
    auto path = write_yaml("minimal.yaml",
        "node_id: \"MINIMAL\"\n"
        "frequency_mhz: 100.0\n"
    );

    auto cfg = load_config(path);
    EXPECT_EQ(cfg.node_id, "MINIMAL");
    EXPECT_DOUBLE_EQ(cfg.frequency_mhz, 100.0);
    EXPECT_FALSE(cfg.encryption_enabled);
    EXPECT_EQ(cfg.max_message_length, 256);
    EXPECT_EQ(cfg.callsign, "UNKNOWN");
    EXPECT_EQ(cfg.network_id, "DEFAULT");
}

TEST_F(ConfigLoaderTest, NonexistentFileThrows) {
    EXPECT_THROW(load_config("/tmp/does_not_exist.yaml"), std::runtime_error);
}

TEST_F(ConfigLoaderTest, MalformedYamlThrows) {
    auto path = write_yaml("bad.yaml",
        "{{{{not valid yaml\n"
    );

    EXPECT_THROW(load_config(path), std::runtime_error);
}
