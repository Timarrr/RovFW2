#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>

namespace Logger{
    void trace(String string);
    void trace(const __FlashStringHelper *string);
    void debug(String string);
    void debug(const __FlashStringHelper *string);
    void info(String string);
    void info(const __FlashStringHelper *string);
    void warn(String string);
    void warn(const __FlashStringHelper *string);
    void error(String string);
    void error(const __FlashStringHelper *string);
};


#endif