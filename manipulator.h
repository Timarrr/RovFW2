#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include "SparkFun_PCA9536_Arduino_Library.h"
#include <Arduino.h>
#include "config.h"

class Manipulator{
public:
    Manipulator(bool launch, bool test);
     
    // @param delta +1 - open, -1 - close
    void setOpenClose(int delta);

    // @param delta +1 - CW, -1 - CCW
    void setRotate(int delta);

    inline void writePCA(int ch, int power);
    inline void set(int ch1, int ch2, int power);

private:
    PCA9536 m_pca;
};

#endif