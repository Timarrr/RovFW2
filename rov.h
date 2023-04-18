/**
 * @file rov.h
 * @author Timarrr (tigrmango@gmail.com)
 * @brief Main file, tying it all together
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef ROV_H
#define ROV_H
#include "cameras.h"
#include "config.h"
#include "imu.h"
#include "isrcontroller.h"
#include "main.h"
#include "manipulator.h"
#include "networking.h"
#include "rovdatatypes.h"
#include "sensors.h"
#include "thrusters.h"
#include "debug.h"

/**
 * @brief ROV class, responsible for controlling all other classes and,
 * subsequently, the ROV
 *
 */
class Rov {
  public:
    /**
     * @brief Construct a new Rov object
     *
     */
    Rov();

    /**
     * @brief Main loop
     *
     */
    void loop();

  private:
    /**
     * @brief Serial communication handler
     */
    void serialHandler();

    /**
     * @brief Thrusters object
     * @see Thrusters
     */
    Thrusters *thrusters;

    /**
     * @brief Cameras object
     * @see Cameras
     */
    Cameras *cameras;

    /**
     * @brief Manipulator object
     * @see Manipulator
     */
    Manipulator *manipulator;

    /**
     * @brief Networking object
     * @see Networking
     */
    Networking *networking;

    /**
     * @brief Sensors object
     * @see Sensors
     */
    Sensors *sensors;

    /**
     * @brief IMUSensor object
     * @see IMUSensor
     */
    IMUSensor *imu;

    /**
     * @brief ISRController object
     * @see ISRController
     */
    ISRController *isr;

    /**
     * @brief RovTelemetry object
     * @see RovTelemetry
     */
    RovTelemetry *tele;

    /**
     * @brief RovControl object
     * @see RovControl
     */
    RovControl *control;

    /**
     * @brief Raw control
     * 
     */
    RovControl *rawControl;

    /**
     * @brief RovAuxControl object
     * @see RovAuxControl
     */
    RovAuxControl *auxControl;

    /**
     * @brief RovRegulators object
     * @see RovRegulators
     */
    RovRegulators *regulators;

    Debug *debug;
};

#endif