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
#include "api/Common.h"
#include <Arduino.h>
#include <cstdint>
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
        constexpr PROGMEM uint8_t rovIP[4] = {192, 168, 1, 5};

        /**
         * @brief ROV's port
         *
         */
        constexpr PROGMEM uint16_t rovPort = 46175;

        /**
         * @brief RC's IP address in an array
         *
         */
        constexpr PROGMEM uint8_t rcIP[4] = {192, 168, 1, 4};

        /**
         * @brief RC's port
         *
         */
        constexpr PROGMEM uint16_t rcPort = 46176;

        /**
         * @brief CS pin for the W5xxx network controller
         *
         */
        constexpr PROGMEM uint8_t netControllerCsPin =
            43; // W5500 CS on PA13 pin
    }           // namespace net

    /**
     * @brief Pins and various settings of the sensors subsystem
     *
     */
    namespace sensors {

        /**
         * @brief Pin of the analog ammeter
         *
         */
        constexpr PROGMEM pin_size_t ammeter_pin = A0;

        /**
         * @brief Pin of the analog voltmeter
         *
         */
        constexpr PROGMEM pin_size_t voltmeter_pin = A1;

        /**
         * @brief Adjusting constant for the ammeter
         *
         */
        constexpr PROGMEM float amperage_multiplier = 0.077821;

        /**
         * @brief Adjusting constant for the voltmeter
         *
         */
        constexpr PROGMEM float voltage_multiplier = 0.016064;

        /**
         * @brief Another adjusting constant for the ammeter
         *
         */
        constexpr PROGMEM int amperage_deflection = 512;
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
        constexpr PROGMEM uint8_t manip_grab_ch = 0;

        /**
         * @brief Manipulator releasing channel
         *
         */
        constexpr PROGMEM uint8_t manip_release_ch = 1;

        /**
         * @brief Manipulator CCW rotation channel
         *
         */
        constexpr PROGMEM uint8_t manip_left_ch = 2;

        /**
         * @brief Manipulator CW rotation channel
         *
         */
        constexpr PROGMEM uint8_t manip_right_ch = 3;
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
        constexpr PROGMEM pin_size_t multiplexer_pin = 10;

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
        constexpr PROGMEM pin_size_t pins[10] = {3, 9, 8, 7, 2, 6, 4, 5};
        /**
         * \brief Thruster directions. Thrusters' values are multiplied by these
         * in Thrusters::update
         */
        constexpr static int8_t thrusterDirections[10] = {1, -1, -1, 1, -1,
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
        constexpr PROGMEM bool waitForSerial = true;

        /**
         * @brief If waitForSerial == true, then for how much does the ROV need
         * to wait for the serial connection?
         *
         */
        constexpr uint16_t waitForSerialTime = 5000; // ms
    }                                                // namespace serial

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
            // clang-format off
            full =              0b0000000000000001,
            fast =              0b0000000000000010,
            standalone =        0b1000000000000000,
            forceEthernet =     0b0100000000000000,
            forceNoEthernet =   0b0010000000000000,
            forceDepth =        0b0001000000000000,
            forceNoDepth =      0b0000100000000000,
            // clang-format on
            initEthernet = full | fast | forceEthernet,
            initDepth = full | fast | forceDepth
        };

        /**
         * @brief Current selected configuration loadout
         *
         */
        constexpr launchConfig currentConfig = launchConfig(fast);
    } // namespace launchConfig

    /**
     * @brief Regulators' coefficients
     *
     */
    namespace regulators {
        /**
         * @brief Depth regulator proportional coefficient
         *
         */
        constexpr PROGMEM float depth_p = 200;

        /**
         * @brief Depth regulator differential coefficient
         *
         */
        constexpr PROGMEM float depth_d = 10000;

        /**
         * @brief Yaw regulator proportional coefficient
         *
         */
        constexpr PROGMEM float yaw_p = 1;

        /**
         * @brief Yaw regulator differential coefficient
         *
         */
        constexpr PROGMEM float yaw_d = 100;

        /**
         * @brief Roll regulator proportional coefficient
         *
         */
        constexpr PROGMEM float roll_p = 1;

        /**
         * @brief Roll regulator differential coefficient
         *
         */
        constexpr PROGMEM float roll_d = 1;

        /**
         * @brief Pitch regulator proportional coefficient
         *
         */
        constexpr PROGMEM float pitch_p = 1;

        /**
         * @brief Pitch regulator differential coefficient
         *
         */
        constexpr PROGMEM float pitch_d = 1;
    } // namespace regulators
} // namespace config

#endif