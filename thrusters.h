/**
 * @file thrusters.h
 * @author Timarrr (tigrmango@gmail.com)
 * @brief Controls the thrusters subsystem
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef THRUSTERS_H
#define THRUSTERS_H
#include "api/Common.h"
#include "isrcontroller.h"
#include "regulators.h"
#include "rovdatatypes.h"
#include <Arduino.h>

/**
 * @brief Class responsible for controlling the thrusters subsystem
 *
 */
class Thrusters {
  public:
    /**
     * @brief Construct a new Thrusters object
     *
     * @param launch Should this subsystem actually launch?
     * @param test Do we need to test the subsystem?
     * @param init_ms_remaining How much do we need to wait for the thrusters to
     * init? Used for "parallalisation" when the ROV inits other subsystems
     * while thrusters do their thing
     */
    Thrusters(bool launch, bool test, long &init_ms_remaining);

    /**
     * @brief Updates thrusters based on RovControl object
     *
     * @param ctrl Pointer to RovControl object
     */
    void update(RovControl ctrl);

  private:
    /**
     * @brief ISRController object
     * @see ISRController
     */
    ISRController m_controller;
};
#endif