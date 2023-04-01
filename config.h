/**
 * @file config.h
 * @author Timarrr (tigrmango@gmail.com)
 * @brief ROV config
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>
/**
 * @brief Config namespace, responsible for the ROV's loadout and pinout
 *
 */
namespace config {
/**
 * @brief Network sub-namespace, responsible for IPs and ports of ROV and RC
 *
 */
namespace net {
/**
 * @brief ROV's IP address in an array
 *
 */
constexpr uint8_t rovIP[4] = {192, 168, 1, 5};

/**
 * @brief ROV's port
 *
 */
constexpr uint16_t rovPort = 3010;

/**
 * @brief RC's IP address in an array
 *
 */
constexpr uint8_t rcIP[4] = {192, 168, 1, 4};

/**
 * @brief RC's port
 *
 */
constexpr uint16_t rcPort = 3010;

/**
 * @brief CS pin for the W5xxx network controller
 *
 */
constexpr uint8_t netControllerCsPin = 43; // W5500 CS on PA13 pin
} // namespace net

/**
 * @brief Pins and various settings of the sensors subsystem
 *
 */
namespace sensors {

/**
 * @brief Pin of the analog ammeter
 *
 */
constexpr int ammeter_pin = A0;

/**
 * @brief Pin of the analog voltmeter
 *
 */
constexpr int voltmeter_pin = A1;

/**
 * @brief Adjusting constant for the ammeter
 *
 */
constexpr float amperage_multiplier = 0.077821;

/**
 * @brief Another adjusting constant for the ammeter
 *
 */
constexpr int amperage_deflection = 512;

/**
 * @brief Adjusting constant for the voltmeter
 *
 */
constexpr float voltage_multiplier = 0.016064;
} // namespace sensors

/**
 * @brief Manipulator channels' settings
 *
 */
namespace manipulator {
/**
 * @brief Manipulator grabbing channel
 *
 */
constexpr int manip_grab_ch = 0;

/**
 * @brief Manipulator releasing channel
 *
 */
constexpr int manip_release_ch = 1;

/**
 * @brief Manipulator CCW rotation channel
 *
 */
constexpr int manip_left_ch = 2;

/**
 * @brief Manipulator CW rotation channel
 *
 */
constexpr int manip_right_ch = 3;
} // namespace manipulator

/**
 * @brief Cameras' subsystem pins
 *
 */
namespace cameras {
/**
 * @brief Multiplexer pin
 *
 */
constexpr int multiplexer_pin = 10;

/**
 * @brief Servos' pins
 *
 */
enum servos { front = A4, back = A5 };
} // namespace cameras

/**
 * @brief Thrusters' subsystem pinout
 *
 */
namespace thrusters {
/**
 * @brief Enum of thrusters
 * @see pins
 */
enum thrusters {
    horizontal_front_left = 0,
    horizontal_front_right,
    horizontal_back_left,
    horizontal_back_right,
    vertical_front_left,
    vertical_front_right,
    vertical_back_left,
    vertical_back_right,
};

/**
 * @brief Pins corresponding to the enum
 *
 */
constexpr int pins[10] = {3, 9, 8, 7, 2, 6, 4, 5};
/*!
 * \brief Thruster directions. Thrusters' values are multiplied by these in
 * Thrusters::update
 */
constexpr static short thrusterDirections[10] = {1, -1, -1, 1, -1,
                                                 1, -1, 1,  1, 1};

} // namespace thrusters

/**
 * @brief Serial debug connection config
 * @todo Implement connection detection proprely
 */
namespace serial {
/**
 * @brief Does the ROV wait for SerialUSB connection to PC?
 *
 */
constexpr bool waitForSerial = true;

/**
 * @brief If waitForSerial == true, then for how much does the ROV need to wait
 * for the serial connection?
 *
 */
constexpr uint16_t waitForSerialTime = 5000; // ms
} // namespace serial

/**
 * @brief Launch configuration loadout
 *
 */
namespace launchConfig {
/**
 * @brief Configuration enum
 * @todo Explain how this works
 */
enum launchConfig {
    full = 0b10000000,
    fast = 0b01000000,
    standalone = 0b00000001,
    forceEthernet = 0b00000010,
    forceNoEthernet = 0b00000100,
    forceDepth = 0b00001000,
    forceNoDepth = 0b00010000,
    initEthernet = full | fast | forceEthernet,
    initDepth = full | fast | forceDepth
};

/**
 * @brief Current selected configuration loadout
 *
 */
constexpr launchConfig currentConfig = launchConfig(fast | forceNoDepth);
} // namespace launchConfig

/**
 * @brief Regulators' coeffs
 *
 */
namespace regulators {
constexpr int depth_p = 1;
constexpr int depth_d = 1;
constexpr int yaw_p = 1;
constexpr int yaw_d = 1;
constexpr int roll_p = 1;
constexpr int roll_d = 1;
constexpr int pitch_p = 1;
constexpr int pitch_d = 1;
} // namespace regulators
} // namespace config

#endif