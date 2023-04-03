#include <Arduino.h>
// #define __cplusplus test
#include <USB/USBAPI.h>
// #undef __cplusplus
#include "syscalls.h"

extern "C" {
int _close(int _) { return -1; }

int _getpid() { return 1; }

int _isatty() { return -1; }

int _fstat(int _) { return -1; }

int _kill(int _, int __) { return -1; }

int _lseek(int _) { return -1; }

int _write(int fd, const char *buf, int len) {
    if (fd == 1 || fd == 2) {
        SerialUSB.write(buf, len);
        return 0;
    } else
        return -1;
}

int _read(int fd, char *buf, size_t count) {
    if (fd == 0) {
        return SerialUSB.readBytes(buf, count);
    } else
        return -1;
}
}