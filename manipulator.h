/**
 * @file manipulator.h
 * @author Timarrr (tigrmango@gmail.com)
 * @brief Controls the manipulator
 * @version 0.1
 * @date 2023-03-31
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "SparkFun_PCA9536_Arduino_Library.h"
#include <Arduino.h>
#include "config.h"

/**
 * @brief Class responsible for controlling the manipulator
 * 
 */
class Manipulator{
public:
    /**
    * @brief Construct a new Manipulator object
    * 
    * @param launch Should this subsystem actually launch?
    * @param test Do we need to test the subsystem? 
    */
    Manipulator(bool launch, bool test);
     
    /**
     * @brief Open/close the manipulator
     * 
     * @param delta >0 - open, <0 - close
     */
    void setOpenClose(int delta);

    // @param delta +1 - CW, -1 - CCW
    /**
     * @brief Rotate the manipulator
     * 
     * @param delta 
     */
    void setRotate(int delta);

private:

    /**
     * @brief PCA interfacing helper
     * 
     * @param ch Channel
     * @param power Power
     */
    inline void writePCA(int ch, int power);

    /**
     * @brief PCA interfacing helper
     * 
     * @param ch1 First channel
     * @param ch2 Second channel
     * @param power Power
     */
    inline void set(int ch1, int ch2, int power);

    /**
    * @brief PCA9536 object
    * 
    */
    PCA9536 *m_pca;

    /**
     * @brief if there's some errors in init then this variable is set to true and manipulator is disabled 
     * 
     */
    bool defunct = false;
};

#endif