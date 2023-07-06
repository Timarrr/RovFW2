#ifndef LOGGER_CPP
#define LOGGER_CPP

#include "logger.h"
#include "USB/USBAPI.h"

namespace Logger {
    void trace(const __FlashStringHelper *string, bool decorator) {
        if (!SerialUSB) return;
        
        if (decorator)
            SerialUSB.print(trace_decorator);
        SerialUSB.print((const char *)string);
        SerialUSB.print(clear_formatting);
    }
    void trace(String string, bool decorator) {
        if (!SerialUSB) return;

        if (decorator)
            SerialUSB.print(trace_decorator);
        SerialUSB.print(string.c_str());
        SerialUSB.print(clear_formatting);
    }

    void debug(const __FlashStringHelper *string, bool decorator) {
        if (!SerialUSB) return;
        
        if (decorator)
            SerialUSB.print(debug_decorator);
        SerialUSB.print((const char *)string);
        SerialUSB.print(clear_formatting);
    }

    void debug(String string, bool decorator) {
        if (!SerialUSB) return;
        
        if (decorator)
            SerialUSB.print(debug_decorator);
        SerialUSB.print(string.c_str());
        SerialUSB.print(clear_formatting);
    }

    void info(const __FlashStringHelper *string, bool decorator) {
        if (!SerialUSB) return;
        
        if (decorator)
            SerialUSB.print(info_decorator);
        SerialUSB.print((const char *)string);
        SerialUSB.print(clear_formatting);
    }

    void info(String string, bool decorator) {
        if (!SerialUSB) return;
        
        if (decorator)
            SerialUSB.print(info_decorator);
        SerialUSB.print(string.c_str());
        SerialUSB.print(clear_formatting);
    }

    void warn(const __FlashStringHelper *string, bool decorator) {
        if (!SerialUSB) return;
        
        if (decorator)
            SerialUSB.print(warn_decorator);
        SerialUSB.print((const char *)string);
        SerialUSB.print(clear_formatting);
    }

    void warn(String string, bool decorator) {
        if (!SerialUSB) return;
        
        if (decorator)
            SerialUSB.print(warn_decorator);
        SerialUSB.print(string.c_str());
        SerialUSB.print(clear_formatting);
    }

    void error(const __FlashStringHelper *string, bool decorator) {
        if (!SerialUSB) return;
        
        if (decorator)
            SerialUSB.print(error_decorator);
        SerialUSB.print((const char *)string);
        SerialUSB.print(clear_formatting);
    }

    void error(String string, bool decorator) {
        if (!SerialUSB) return;
        
        if (decorator)
            SerialUSB.print(error_decorator);
        SerialUSB.print(string.c_str());
        SerialUSB.print(clear_formatting);
    }
} // namespace Logger

#endif