#ifndef THRUSTERS_H
#define THRUSTERS_H
#include <Arduino.h>
#include "rovdatatypes.h"
#include "isrcontroller.h"

class Thrusters{
public:
    Thrusters();
    Thrusters(bool launch, bool test);
    void init();
    void test();
    void update_thrusters(RovControl & ctrl);
private:
    ISRController m_controller;
};

#endif