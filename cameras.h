/**
 * @file cameras.h
 * @author Timarrr (tigrmango@gmail.com)
 * @brief Rotating cameras
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef CAMERAS_H
#define CAMERAS_H

#include "config.h"
#include "logger.h"
#include <Arduino.h>
#include <Servo.h>

/**
 * @brief Class responsible for rot(a)ting cameras on the ROV
 *
 */
class Cameras {
  public:
    /**
     * @brief Construct a new Cameras object
     *
     * @param launch Should this subsystem actually launch?
     * @param test Do we need to test the subsystem?
     */
    Cameras(bool launch, bool test = false);

    /**
     * @brief Set the angle delta for the selected camera
     *
     * @param idx Camera index
     * @param angle Angle delta
     */
    void set_angle_delta(int idx, int angle);

    /**
     * @brief Select camera by index
     *
     * @param index Camera index
     */
    void select_cam(int index);

    /**
     * @brief Get camera index
     *
     * @return int Index
     */
    int get_cam_index();

  private:
    /**
     * @brief Camera angles
     *
     */
    int angles[2] = {90, 90};

    /**
     * @brief Servo object for the front camera
     *
     */
    Servo m_frontServo;

    /**
     * @brief Servo object for the back camera
     *
     */
    Servo m_backServo;

    /**
     * @brief Last angle change times, used to slowdown movement
     *
     */
    long long m_lastUpdateTimes[2] = {0, 0};

    /**
     * @brief if set to true returns from every function early
     *
     */
    bool inactive = false;
};

#endif