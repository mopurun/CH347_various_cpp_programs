#pragma comment(lib, "CH347DLLA64.LIB")
#include <windows.h>
#include <stdio.h>
#include "CH347DLL.H"
unsigned long  serialnum = 0;
unsigned char bufferwr[4096];
unsigned char bufferrd[4096];

int main(void)
{
    CH347OpenDevice(serialnum);
    CH347I2C_Set(serialnum,2);   

    //write to 0x10
    bufferwr[0]=0xBA;
    bufferwr[1]=0x10;
    bufferwr[2]=0xA0;    
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);       

    unsigned int i ;
    for(i=0;i<200;i++){
        Sleep(100);
        bufferwr[1]=0x28;
        CH347StreamI2C(serialnum,2,bufferwr,1,bufferrd);       
    //    printf("PRESS_OUT_XL:%02X\n",bufferrd[0]);
        unsigned int PRESS_OUT_XL = bufferrd[0];
        bufferwr[1]=0x29;
        CH347StreamI2C(serialnum,2,bufferwr,1,bufferrd);       
    //    printf("PRESS_OUT_L:%02X\n",bufferrd[0]);
        unsigned int PRESS_OUT_L = bufferrd[0];    
        bufferwr[1]=0x2A;
        CH347StreamI2C(serialnum,2,bufferwr,1,bufferrd);       
    //    printf("PRESS_OUT_H:%02X\n",bufferrd[0]);    
        unsigned int PRESS_OUT_H = bufferrd[0];

        float PRESS_OUT = (PRESS_OUT_H *65536+ PRESS_OUT_L *256 + PRESS_OUT_XL)/4096.0;
        printf("PRESS_OUT:%4.2fhPa\r",PRESS_OUT);
    }
    printf("\n");
    CH347CloseDevice(serialnum);
    return 0;
}