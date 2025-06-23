#pragma comment(lib, "CH347DLLA64.LIB")
#include <windows.h>
#include <stdio.h>
#include "CH347DLL.H"

int main(void)
{
    unsigned long  serialnum = 0;
    CH347OpenDevice(serialnum);

    CH347I2C_Set(serialnum,2);   
    unsigned char bufferwr[4096];
    unsigned char bufferrd[4096];
    bufferwr[0]=0xBA;
    bufferwr[1]=0x0F;
    CH347StreamI2C(serialnum,2,bufferwr,1,bufferrd);       
    printf("%02X\n",bufferrd[0]);
    CH347CloseDevice(serialnum);
    return 0;

}