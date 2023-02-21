#ifndef ROVDATATYPES_H
#define ROVDATATYPES_H

#include <Arduino.h>

struct RovControl
{
    int8_t header = 0xAC;
    uint8_t version = 2;
    uint8_t auxFlags = 0b00000000;
    int8_t thrusterPower[10] = {0,0,0,0,0,0,0,0,0,0};
    int8_t cameraRotation[2] = {0,0}; // front, rear
    int8_t manipulator[2] = {0,0}; // open/close, rotate
    int8_t camsel = false;
    RovControl(){}
};

struct RovTelemetry
{
    static const uint8_t header_telemetry = 0xAE;
    enum ErrorCode{
        NoError,
        WrongCrc //TODO: maybe add smth else
    };
    uint8_t header = 0;
    int8_t version = 2;
    float depth = 0.0f;
    float pitch = 0; //! -180/180;
    float yaw = 0; //! 0 - 360;
    float roll = 0; //! -180/180;
    float ammeter = 0.0f;
    float voltmeter = 0.0f;
    int8_t cameraIndex = 0; //! 0 / 1 video multiplexer
    float temperature = 0.0f;
    ErrorCode ec = NoError;
    RovTelemetry() {}
};

#endif // ROVDATATYPES_H
