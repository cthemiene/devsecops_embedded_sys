#include <gtest/gtest.h>
#include "comms.h"

class CommsModuleTest : public ::testing::Test {
protected:
    CommsConfig valid_config() {
        return CommsConfig{
            "ALPHA-7",   // node_id
            245.5,       // frequency_mhz
            true,        // encryption_enabled
            256,         // max_message_length
            "VIPER",     // callsign
            "TACNET-9"   // network_id
        };
    }
};

// --- validate_config tests ---

TEST_F(CommsModuleTest, ValidConfigReturnsOK) {
    CommsModule comms(valid_config());
    EXPECT_EQ(comms.validate_config(), CommsStatus::OK);
}

TEST_F(CommsModuleTest, EmptyNodeIdReturnsInvalidConfig) {
    auto cfg = valid_config();
    cfg.node_id = "";
    CommsModule comms(cfg);
    EXPECT_EQ(comms.validate_config(), CommsStatus::INVALID_CONFIG);
}

TEST_F(CommsModuleTest, FrequencyBelowMinReturnsOutOfRange) {
    auto cfg = valid_config();
    cfg.frequency_mhz = 29.9;
    CommsModule comms(cfg);
    EXPECT_EQ(comms.validate_config(), CommsStatus::FREQUENCY_OUT_OF_RANGE);
}

TEST_F(CommsModuleTest, FrequencyAtMinBoundaryReturnsOK) {
    auto cfg = valid_config();
    cfg.frequency_mhz = 30.0;
    CommsModule comms(cfg);
    EXPECT_EQ(comms.validate_config(), CommsStatus::OK);
}

TEST_F(CommsModuleTest, FrequencyAboveMaxReturnsOutOfRange) {
    auto cfg = valid_config();
    cfg.frequency_mhz = 6000.1;
    CommsModule comms(cfg);
    EXPECT_EQ(comms.validate_config(), CommsStatus::FREQUENCY_OUT_OF_RANGE);
}

TEST_F(CommsModuleTest, FrequencyAtMaxBoundaryReturnsOK) {
    auto cfg = valid_config();
    cfg.frequency_mhz = 6000.0;
    CommsModule comms(cfg);
    EXPECT_EQ(comms.validate_config(), CommsStatus::OK);
}

TEST_F(CommsModuleTest, ZeroMaxMessageLengthReturnsInvalidConfig) {
    auto cfg = valid_config();
    cfg.max_message_length = 0;
    CommsModule comms(cfg);
    EXPECT_EQ(comms.validate_config(), CommsStatus::INVALID_CONFIG);
}

TEST_F(CommsModuleTest, NegativeMaxMessageLengthReturnsInvalidConfig) {
    auto cfg = valid_config();
    cfg.max_message_length = -1;
    CommsModule comms(cfg);
    EXPECT_EQ(comms.validate_config(), CommsStatus::INVALID_CONFIG);
}

// --- transmit_message tests ---

TEST_F(CommsModuleTest, TransmitValidMessageReturnsOK) {
    CommsModule comms(valid_config());
    EXPECT_EQ(comms.transmit_message("Hello"), CommsStatus::OK);
}

TEST_F(CommsModuleTest, TransmitEmptyMessageReturnsEmptyMessage) {
    CommsModule comms(valid_config());
    EXPECT_EQ(comms.transmit_message(""), CommsStatus::EMPTY_MESSAGE);
}

TEST_F(CommsModuleTest, TransmitOversizedMessageReturnsTooLong) {
    auto cfg = valid_config();
    cfg.max_message_length = 5;
    CommsModule comms(cfg);
    EXPECT_EQ(comms.transmit_message("123456"), CommsStatus::MESSAGE_TOO_LONG);
}

TEST_F(CommsModuleTest, TransmitMessageAtExactLimitReturnsOK) {
    auto cfg = valid_config();
    cfg.max_message_length = 5;
    CommsModule comms(cfg);
    EXPECT_EQ(comms.transmit_message("12345"), CommsStatus::OK);
}

// --- status_to_string tests ---

TEST_F(CommsModuleTest, StatusToStringCoversAllValues) {
    EXPECT_EQ(status_to_string(CommsStatus::OK), "OK");
    EXPECT_EQ(status_to_string(CommsStatus::INVALID_CONFIG), "INVALID_CONFIG");
    EXPECT_EQ(status_to_string(CommsStatus::MESSAGE_TOO_LONG), "MESSAGE_TOO_LONG");
    EXPECT_EQ(status_to_string(CommsStatus::EMPTY_MESSAGE), "EMPTY_MESSAGE");
    EXPECT_EQ(status_to_string(CommsStatus::FREQUENCY_OUT_OF_RANGE), "FREQUENCY_OUT_OF_RANGE");
}

// --- get_config tests ---

TEST_F(CommsModuleTest, GetConfigReturnsOriginalValues) {
    auto cfg = valid_config();
    CommsModule comms(cfg);
    const auto& returned = comms.get_config();
    EXPECT_EQ(returned.node_id, "ALPHA-7");
    EXPECT_DOUBLE_EQ(returned.frequency_mhz, 245.5);
    EXPECT_TRUE(returned.encryption_enabled);
    EXPECT_EQ(returned.max_message_length, 256);
    EXPECT_EQ(returned.callsign, "VIPER");
    EXPECT_EQ(returned.network_id, "TACNET-9");
}
