#ifndef LOGGER_CPP
#define LOGGER_CPP

#include "logger.h"
#include "USB/USBAPI.h"

namespace Logger {
    void trace(const __FlashStringHelper *string, bool decorator) {
        if (decorator)
            printf(trace_decorator);
        printf((const char *)string);
    }
    void trace(String string, bool decorator) {
        if (decorator)
            printf(trace_decorator);
        printf(string.c_str());
    }

    void debug(const __FlashStringHelper *string, bool decorator) {
        if (decorator)
            printf(debug_decorator);
        printf((const char *)string);
    }

    void debug(String string, bool decorator) {
        if (decorator)
            printf(debug_decorator);
        printf(string.c_str());
    }

    void info(const __FlashStringHelper *string, bool decorator) {
        if (decorator)
            printf(info_decorator);
        printf((const char *)string);
    }

    void info(String string, bool decorator) {
        if (decorator)
            printf(info_decorator);
        printf(string.c_str());
    }

    void warn(const __FlashStringHelper *string, bool decorator) {
        if (decorator)
            printf(warn_decorator);
        printf((const char *)string);
    }

    void warn(String string, bool decorator) {
        if (decorator)
            printf(warn_decorator);
        printf(string.c_str());
    }

    void error(const __FlashStringHelper *string, bool decorator) {
        if (decorator)
            printf(error_decorator);
        printf((const char *)string);
    }

    void error(String string, bool decorator) {
        if (decorator)
            printf(error_decorator);
        printf(string.c_str());
    }
} // namespace Logger

#endif