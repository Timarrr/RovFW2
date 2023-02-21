#ifndef HELPERS_H
#define HELPERS_H
#include "rovdatatypes.h"
#include <Arduino.h>
namespace helpers {
    template<typename T>
    inline void read_bytes(const uint8_t *msg, size_t &i, T &data){
        memcpy(&data, msg + i, sizeof(data));
        i += sizeof(data);
    }

    template<typename T>
    inline void write_bytes(uint8_t *msg, size_t &i, T &data){
        memcpy(msg + i, &data, sizeof(data));
        i += sizeof(data);
    }

    template <typename T>
        inline T swapEndian(T u) {

            union
            {
                T u;
                unsigned char u8[sizeof(T)];
            } source, dest;

            source.u = u;

            for (size_t k = 0; k < sizeof(T); k++)
                dest.u8[k] = source.u8[sizeof(T) - k - 1];

            return dest.u;
        }

    void swapEndianRovControl(RovControl & rc);

    void swapEndianRovTelemetry(RovTelemetry & rt);

}

#endif
