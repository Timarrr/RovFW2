#ifndef CONFIG_H
#define CONFIG_H
#include <Arduino.h>

namespace config {
    namespace net {
        constexpr uint8_t rovIP[4] = {192, 168, 1, 5};
        constexpr uint16_t rovPort = 3010;
        constexpr uint8_t rcIP[4] = {192, 168, 1, 4};
        constexpr uint16_t rcPort = 3010;
        constexpr uint8_t netControllerCsPin = 43; // W5500 CS on PA13 pin
    }
    namespace sensors {
        constexpr int ammeter_pin = A0;
        constexpr int voltmeter_pin = A1;

        constexpr double voltage_multiplier  = 0.016064;
        constexpr double amperage_multiplier = 0.077821;
        constexpr double amperage_deflection = 512.0;
    }
    namespace manipulator {
        constexpr int manip_grab_ch     = 0;
        constexpr int manip_release_ch  = 1;

        constexpr int manip_left_ch     = 2;
        constexpr int manip_right_ch    = 3;
    }
    namespace cameras {
        constexpr int multiplexer_pin = 10;
        enum servos {
            front = A4,
            back = A5,
            pwm_a2 = 2,
            pwm_a3 = 3,
        };
    }
    namespace thrusters {
        constexpr int pins[10] = {3,9,8,7,2,6,4,5};
        enum thrusters {
            horizontal_front_left = 0,
            horizontal_front_right,
            horizontal_back_left,
            horizontal_back_right,
            vertical_front_left,
            vertical_front_right,
            vertical_back_left,
            vertical_back_right,
            custom_0,
            custom_1,
        };
    }
    namespace serial {
        constexpr bool waitForSerial = 1;
        constexpr uint16_t waitForSerialTime = 5000; //ms
    }
    namespace launchConfig {
        enum configType{
            full = 0b10000000,
            standalone = 0b00000001,
            standaloneWithEthernet = 0b00000010,
            standaloneWithDepth = 0b00000100,
            fast = 0b00001000
        };    
        constexpr configType currentConfig = full;
    }

}

#endif