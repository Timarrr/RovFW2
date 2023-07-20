/**
 * @file isrcontroller.h
 * @author Timarrr (tigrmango@gmail.com)
 * @brief Interface with SAMD_ISR_Servos library
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef ISRCONTROLLER_H
#define ISRCONTROLLER_H
#include "SAMD_ISR_Servo/SAMD_ISR_Servo.h"
#include "config.h"
#include "logger.h"
#include <Servo.h>
#include <Wire.h>

/**
 * @brief Class responsible for interfacing with the ISR
 *
 */
class ISRController {
  public:
    /**
     * @brief Construct a new ISRController object
     *
     */
    ISRController();
    /**
     * @brief Set selected thruster power
     *
     * @param idx index of the thruster
     * @see config::thrusters::thrusters
     * @param power Power from -100 to 100 to set thruster to (other values are
     * clamped to avoid breakage)
     */
    void setThruster(int idx, int power);

  private:
    /**
     * @brief Minimum (full backward) thrust in microseconds(?)
     * @todo research and clarify this
     *
     */
    static constexpr int pulse_min[8] = {1000,1000,1000,1000,1000,1000,1000,1000};

    /**
     * @brief Medium (stop/neutral) thrust in microsesonds(?)
     * @todo research and clarify this
     *
     */
    static constexpr int pulse_med[8] = {1488,1488,1488,1488,1488,1488,1488,1488};

    /**
     * @brief Maximum (full forward) thrust in microsesonds(?)

     * @todo research and clarify this
     *
     */
    static constexpr int pulse_max[8] = {2000,2000,2000,2000,2000,2000,2000,2000};

    /**
     * @brief ISR pins
     *
     */
    int m_pins[8] = {0, 0, 0, 0, 0, 0, 0, 0};

    /**
     * @brief ISR servos
     *
     */
    int m_isrServos[8] = {0, 0, 0, 0, 0, 0, 0, 0};
};

#endif