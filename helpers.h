/**
 * @file helpers.h
 * @author Timarrr (tigrmango@gmail.com)
 * @brief Various helpers for reading and writing data
 * @version 0.1
 * @date 2023-03-31
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef HELPERS_H
#define HELPERS_H
#include "logger.h"
#include "rovdatatypes.h"
#include <Arduino.h>

/**
 * @brief Various helpers for reading and writing data
 *
 */
namespace helpers {

    /**
     * @brief Reads sizeof(data) bytes from msg to data
     *
     * @tparam T Type
     * @param msg Input buffer
     * @param i Reference to "counter" variable
     * @param data Reference to the output variable
     */
    template <typename T>
    void read_bytes(const uint8_t *msg, size_t &i, T &data) {
        memcpy(&data, msg + i, sizeof(data));
        i += sizeof(data);
    }

    /**
     * @brief Writes sizeof(data) bytes from data to msg
     *
     * @tparam T Type
     * @param msg Output buffer
     * @param i Reference to "counter" variable
     * @param data Reference to the input variable
     */
    template <typename T> void write_bytes(uint8_t *msg, size_t &i, T &data) {
        memcpy(msg + i, &data, sizeof(data));
        i += sizeof(data);
    }

    /**
     * @brief Swaps bytes of multi-byte data
     * @todo Research into this and deprecate this shit
     * @tparam T Type
     * @param u Value to swap endianness in which
     * @return T Value with swapped endianness
     */
    template <typename T> inline T swapEndian(T u) {

        union {
            T u;
            unsigned char u8[sizeof(T)];
        } source, dest;

        source.u = u;

        for (size_t k = 0; k < sizeof(T); k++)
            dest.u8[k] = source.u8[sizeof(T) - k - 1];

        // Logger::trace(String(dest.u, 10));
        return dest.u;
    }

    template <typename T> inline T clamp(T val, T min, T max) {
        return val > max ? max : val < min ? min : val;
    }

    // template <typename T>
    //     T swapEndian(T u) {

    //     }

    /**
     * @brief Swaps endianness \b {in-place} in provided RovControl
     *
     * @param rc RovControl variable
     */
    void swapEndianRovControl(RovControl &rc);

    /**
     * @brief Swaps endianness \b {in-place} in provided RovTelemetry
     *
     * @param rt RovTelemetry variable
     */
    void swapEndianRovTelemetry(RovTelemetry &rt);

} // namespace helpers

#endif
