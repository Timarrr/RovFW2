#include <Arduino.h>
// #define __cplusplus test
#include <USB/USBAPI.h>
// #undef __cplusplus
#include "syscalls.h"

extern "C" {
int _close(int _) { SerialUSB.println("_close() called with " + String(_)); return -1; }

int _getpid() { SerialUSB.println("_getpid() called"); return 1; }

int _isatty() { SerialUSB.println("_isatty() called"); return -1; }

int _fstat(int _) {return 0; }

int _kill(int _, int __) { SerialUSB.println("_kill() called with " + String(_) + ";" +String(__)); return -1; }

int _lseek(int _) { SerialUSB.println("_lseek() called with " + String(_)); return -1; }

int _write(int fd, const char *buf, int len) {
    if (fd == 1 || fd == 2) {
        SerialUSB.println(String(len) + "   " + String(SerialUSB.availableForWrite()));
        SerialUSB.flush();
        SerialUSB.write(buf);
        SerialUSB.flush();
        // return 0;
    } else
        return -1;
}

int _read(int fd, char *buf, size_t count) {
    SerialUSB.println("_read() called with fd " + String(fd));

    if (fd == 0 && SerialUSB.available()) {
        return SerialUSB.readBytes(buf, count);
    } else
        return 0;
}
}