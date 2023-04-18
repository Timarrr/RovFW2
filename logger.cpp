#ifndef LOGGER_CPP
#define LOGGER_CPP

#include "logger.h"
#include "USB/USBAPI.h"

namespace Logger {
    void trace(const __FlashStringHelper *string, bool decorator) {
        if (decorator)
            SerialUSB.print(trace_decorator);
        SerialUSB.print((const char *)string);
    }
    void trace(String string, bool decorator) {
        if (decorator)
            SerialUSB.print(trace_decorator);
        SerialUSB.print(string.c_str());
    }

    void debug(const __FlashStringHelper *string, bool decorator) {
        if (decorator)
            SerialUSB.print(debug_decorator);
        SerialUSB.print((const char *)string);
    }

    void debug(String string, bool decorator) {
        if (decorator)
            SerialUSB.print(debug_decorator);
        SerialUSB.print(string.c_str());
    }

    void info(const __FlashStringHelper *string, bool decorator) {
        if (decorator)
            SerialUSB.print(info_decorator);
        SerialUSB.print((const char *)string);
    }

    void info(String string, bool decorator) {
        if (decorator)
            SerialUSB.print(info_decorator);
        SerialUSB.print(string.c_str());
    }

    void warn(const __FlashStringHelper *string, bool decorator) {
        if (decorator)
            SerialUSB.print(warn_decorator);
        SerialUSB.print((const char *)string);
    }

    void warn(String string, bool decorator) {
        if (decorator)
            SerialUSB.print(warn_decorator);
        SerialUSB.print(string.c_str());
    }

    void error(const __FlashStringHelper *string, bool decorator) {
        if (decorator)
            SerialUSB.print(error_decorator);
        SerialUSB.print((const char *)string);
    }

    void error(String string, bool decorator) {
        if (decorator)
            SerialUSB.print(error_decorator);
        SerialUSB.print(string.c_str());
    }
} // namespace Logger

#endif