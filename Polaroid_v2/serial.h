#include<windows.h>

bool WriteComPort(LPCSTR PortSpecifier, LPCVOID data);
int ReadByte(LPCSTR PortSpecifier);