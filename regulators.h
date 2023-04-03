/**
 * @file regulators.h
 * @author Timarrr (tigrmango@gmail.com)
 * @brief Int-ified PD regulator
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#include "rovdatatypes.h"
#include "samd21/include/component/mtb.h"
#include <Arduino.h>

/**
 * @brief Class responsible for implementation of the PD regulator
 *
 */
class PDRegulator {
  public:
    /**
     * @brief Construct a new PDRegulator object
     *
     * @param p_coeff kP, proportional coefficent
     * @param d_coeff kD, differential coefficent
     */
    PDRegulator(int p_coeff, int d_coeff);

    /**
     * @brief Evaluate the regulator expression
     *
     * @param data Data
     * @param target Target
     * @return int Control signal
     */
    int eval(int data, int target);

  private:
    /**
     * @brief kP, proportional coefficent
     *
     */
    int kP;

    /**
     * @brief kD, differential coefficent
     *
     */
    int kD;

    /**
     * @brief last error, used for the derivative part
     *
     */
    int lastErr;
    /**
     * @brief last run time, used for the derivative part
     *
     */
    uint32_t lastTime;
};

/**
 * @brief Floating point PD regulator implementation
 *
 */
class FPPDRegulator {
  public:
    /**
     * @brief Construct a new FPPDRegulator object
     *
     * @param coeffP Proportional coefficient
     * @param coeffD Differential coefficient
     */
    FPPDRegulator(float coeffP, float coeffD)
        : kP(coeffP), kD(coeffD), lastError(0.0f), lastTime(0){};
    /**
     * @brief Evaluate the regulator expression
     *
     * @param data Data
     * @param target Target
     * @return float Control signal
     */
    float eval(float data, float target);

  private:
    float kP;

    float kD;

    float lastError;

    uint32_t lastTime;
};

/**
 * @brief Regulators' wrapper for modifying RovControl
 *
 */
class RovRegulators {
  public:
    /**
     * @brief Construct a new Rov Regulators object
     *
     */
    RovRegulators();
    /**
     * @brief Evaluate regultors
     *
     * @param ctrl
     * @param auxCtrl
     * @param tele
     */
    void evaluate(RovControl &ctrl, RovAuxControl &auxCtrl, RovTelemetry &tele);

  private:
    /**
     * @brief Depth regulators
     *
     */
    PDRegulator depthReg;

    /**
     * @brief Yaw regulator
     *
     */
    PDRegulator yawReg;

    /**
     * @brief Roll regulator
     *
     */
    PDRegulator rollReg;

    /**
     * @brief Pitch regulator
     *
     */
    PDRegulator pitchReg;
};