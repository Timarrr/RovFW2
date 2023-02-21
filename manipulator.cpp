#ifndef MANIPULATOR_CPP
#define MANIPULATOR_CPP

#include "manipulator.h"
Manipulator::Manipulator(bool launch, bool test){
    if (m_pca.begin() == false) {
        SerialUSB.println("PCA9536 not detected or failed to init! Check the I2C connection and reset the ROV");
    }

    for (int i = 0; i < 4; i++) {
        m_pca.pinMode(i, OUTPUT);
        m_pca.write(i, LOW);
    }
}

void Manipulator::setOpenClose(int delta){
    using namespace config::manipulator;
    int maxSpeed = 100;
    set(manip_release_ch, manip_grab_ch, constrain(delta * 100, -maxSpeed, maxSpeed));
}

void Manipulator::setRotate(int delta){
    using namespace config::manipulator;
    int maxSpeed = 100;
    set(manip_left_ch, manip_right_ch, constrain(delta * 100, -maxSpeed, maxSpeed));
}

inline void Manipulator::set(int ch1, int ch2, int power) {
    if (power == 0) {
        writePCA(ch1, 0);
        writePCA(ch2, 0);
        return;
    }
    if (power > 0) {
        writePCA(ch1, power);
        writePCA(ch2, 0);
        return;
    }
    if (power < 0) {
        writePCA(ch1, 0);
        writePCA(ch2, power);
        return;
    }
}
inline void Manipulator::writePCA(int ch, int power){
    int state = LOW;
    if (abs(power) >= 50) {
        state = HIGH;
    }
    m_pca.write(ch, state);
}
#endif
