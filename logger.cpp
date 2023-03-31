#ifndef LOGGER_CPP
#define LOGGER_CPP

#include "logger.h"
#include "USB/USBAPI.h"


namespace Logger {
    void trace(const __FlashStringHelper *string){
        SerialUSB.print(trace_decorator); SerialUSB.println(string);
    }    
    void trace(String string){
        SerialUSB.print(trace_decorator); SerialUSB.println(string);
    }

    void debug(const __FlashStringHelper *string){
        SerialUSB.print(debug_decorator); SerialUSB.println(string);
    }

    void debug(String string){
        SerialUSB.print(debug_decorator); SerialUSB.println(string);
    }

    void info(const __FlashStringHelper *string){
        SerialUSB.print(info_decorator); SerialUSB.println(string);
    }

    void info(String string){
        SerialUSB.print(info_decorator); SerialUSB.println(string);
    }

    void warn(const __FlashStringHelper *string){
        SerialUSB.print(warn_decorator); SerialUSB.println(string);
    }

    void warn(String string){
        SerialUSB.print(warn_decorator); SerialUSB.println(string);
    }

    void error(const __FlashStringHelper *string){
        SerialUSB.print(error_decorator); SerialUSB.println(string);
    }

    void error(String string){
        SerialUSB.print(error_decorator); SerialUSB.println(string);
    }
}


#endif