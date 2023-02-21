#ifndef MAIN_CPP
#define MAIN_CPP

#include "main.h"

Rov rov = Rov();

void setup(){
    Wire.begin();
    Wire.setTimeout(1000);
    Wire.setClock(10000);

    delay(100);

    SerialUSB.begin(115200);
    unsigned int t_on = millis();
    while(!SerialUSB && config::serial::waitForSerial){
      if(millis() >= t_on + config::serial::waitForSerialTime){  
        break;
      }else{
        analogWrite(LED_BUILTIN, sin(millis() * 0.03) * 127 + 127);
        delay(1);
      }
    }
    if(config::serial::waitForSerial){
      delay(200);
    }			
    analogWrite(LED_BUILTIN, 255);
}

void loop(){
    rov.loop();
}

#endif