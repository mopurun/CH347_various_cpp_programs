#pragma comment(lib, "CH347DLLA64.LIB")
#include <windows.h>
#include <stdio.h>
#include "CH347DLL.H"


unsigned long  serialnum = 0;
unsigned char bufferwr[4096] = {};
unsigned char bufferrd[4096] = {};


int main(void)
{
    CH347OpenDevice(serialnum);
    
    bool result = CH347I2C_Set(serialnum,2);
    if(!result){
        printf("Device not found.\n",result);
        return 0;        
    }   



    bufferwr[0]=0x7C;
    bufferwr[1]=0x00;


    bufferwr[2]=0x38;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);

    bufferwr[2]=0x39;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);

    bufferwr[2]=0x14;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);

    bufferwr[2]=0x70;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);

    bufferwr[2]=0x56;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);

    bufferwr[2]=0x6C;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);



    bufferwr[2]=0x38;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);

    bufferwr[2]=0x0C;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);

    bufferwr[2]=0x01;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);    

    Sleep(100);

    bufferwr[1]=0x40;
    const char* mopurun_str="@mopurun";
    for (int i=0; i < sizeof(mopurun_str)/sizeof(mopurun_str[0]);i++){
        bufferwr[2]=mopurun_str[i];
        CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd); 
    }    

    //set 40h
    bufferwr[1]=0x00;    
    bufferwr[2]=0xC0;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);  

    //Sleep(100);

    bufferwr[1]=0x40;
    const char* elec_str="\x5F""elec";
    for (int i=0; i < sizeof(elec_str)/sizeof(elec_str[0]);i++){
        bufferwr[2]=elec_str[i];
        CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd); 
    }        



    printf("\n");
    CH347CloseDevice(serialnum);
    return 0;
}