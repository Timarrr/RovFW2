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
#include <Arduino.h>

class PDRegulator{
public:
    PDRegulator(int p_comp, int d_comp);
    int eval(int data, int target);

private:
    int kP;
    int kD;
    int lastErr;
    uint32_t lastTime;
};

class RovRegulators{
public:
    RovRegulators();
    void evaluate(RovControl &ctrl, RovAuxControl &auxCtrl, RovTelemetry &tele);
private:
    PDRegulator depthReg;
    PDRegulator yawReg;
    PDRegulator rollReg;
    PDRegulator pitchReg;
};