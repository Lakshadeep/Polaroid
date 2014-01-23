#ifndef SERIALPORT_H
#define SERIALPORT_H
#include <windows.h>
#include <cstdio>

enum Baudrate {
    B50	= 50,
    B110	= 110,
    B150	= 150,
    B300	= 300,
    B1200	= 1200,
    B2400	= 2400,
    B4800	= 4800,
    B9600   = 9600,
    B19200	= 19200,
    B38400	= 38400,
    B57600	= 57600,
    B115200 = 115200,
    B230400	= 230400,
    B460800	= 460800,
    B500000 = 500000,
    B1000000= 1000000
};

enum Stopbits {
    one = ONESTOPBIT,
    onePointFive = ONE5STOPBITS,
    two = TWOSTOPBITS
};

enum Paritycheck {
    even = EVENPARITY,
    odd  = ODDPARITY,
    off  = NOPARITY,
    mark = MARKPARITY
};


void ErrorExit(LPTSTR lpszFunction);
HANDLE openSerialPort(LPCSTR portname,enum Baudrate baudrate, enum Stopbits stopbits, enum Paritycheck parity);
DWORD readFromSerialPort(HANDLE hSerial, char * buffer, int buffersize);
DWORD writeToSerialPort(HANDLE hSerial, char * data, int length);
void closeSerialPort(HANDLE hSerial);
#endif
