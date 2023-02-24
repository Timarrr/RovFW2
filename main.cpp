#ifndef MAIN_CPP
#define MAIN_CPP

#include "main.h"

Rov *rov;

void setup(){	
    analogWrite(LED_BUILTIN, 255);
    rov = new Rov();
}

void loop(){
    rov->loop();
}

#endif