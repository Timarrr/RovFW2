#ifndef LOGGER_CPP
#define LOGGER_CPP

#include "logger.h"
#include "USB/USBAPI.h"
namespace Logger {
    void trace(const __FlashStringHelper *string){
        SerialUSB.print(F("TRCE: ")); SerialUSB.println(string);
    }    
    void trace(String string){
        SerialUSB.print(F("TRCE: ")); SerialUSB.println(string);
    }

    void debug(const __FlashStringHelper *string){
        SerialUSB.print(F("DEBG: ")); SerialUSB.println(string);
    }

    void debug(String string){
        SerialUSB.print(F("DEBG: ")); SerialUSB.println(string);
    }

    void info(const __FlashStringHelper *string){
        SerialUSB.print(F("INFO: ")); SerialUSB.println(string);
    }

    void info(String string){
        SerialUSB.print(F("INFO: ")); SerialUSB.println(string);
    }

    void warn(const __FlashStringHelper *string){
        SerialUSB.print(F("WARN: ")); SerialUSB.println(string);
    }

    void warn(String string){
        SerialUSB.print(F("WARN: ")); SerialUSB.println(string);
    }

    void error(const __FlashStringHelper *string){
        SerialUSB.print(F("ERR!: ")); SerialUSB.println(string);
    }

    void error(String string){
        SerialUSB.print(F("ERR!: ")); SerialUSB.println(string);
    }
}


#endif