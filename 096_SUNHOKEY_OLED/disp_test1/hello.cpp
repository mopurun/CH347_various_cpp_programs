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



    bufferwr[0]=0x78;
    bufferwr[1]=0x00;

    //Display OFF
    bufferwr[2]=0xAE;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);

    //clk divide ratio and osc freq at p40
    bufferwr[2]=0xD5;
    bufferwr[3]=0x80;
    CH347StreamI2C(serialnum,4,bufferwr,0,bufferrd);

    //Multiplex ratio default at p37
    bufferwr[2]=0xA8;
    bufferwr[3]=0x3F;
    CH347StreamI2C(serialnum,4,bufferwr,0,bufferrd);

    //display offset
    bufferwr[2]=0xD3;
    bufferwr[3]=0x00;
    bufferwr[4]=0x40;
    CH347StreamI2C(serialnum,5,bufferwr,0,bufferrd);

    //charge bump enable //p62
    bufferwr[2]=0x8D;
    bufferwr[3]=0x14;
    CH347StreamI2C(serialnum,4,bufferwr,0,bufferrd);    

    //memory addressing mode = Horizontal Addressing Mode p30,p34
    bufferwr[2]=0x20;
    bufferwr[3]=0x00;
    CH347StreamI2C(serialnum,4,bufferwr,0,bufferrd);  

    //column address 127 is mapped to SEG0 //p36,p31
    bufferwr[2]=0xA1;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd); 


    //com remapped mode p31
    bufferwr[2]=0xC8;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);

    //com pins p40,p31
    bufferwr[2]=0xDA;
    bufferwr[3]=0x12;    
    CH347StreamI2C(serialnum,4,bufferwr,0,bufferrd);  

    //display start line 0
    bufferwr[2]=0x40;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd); 

    //resume to RAM content display  p28
    bufferwr[2]=0xA4;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd); 

    //Normal display
    bufferwr[2]=0xA6;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd); 

    //Deactivate scroll
    bufferwr[2]=0x2E;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd); 

    //Display ON
    bufferwr[2]=0xAF;    
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd); 

    //Set Page Address 
    bufferwr[2]=0x22;
    bufferwr[3]=0x00;
    bufferwr[4]=0x07;
    CH347StreamI2C(serialnum,5,bufferwr,0,bufferrd);

    //Set Column Address 
    bufferwr[2]=0x21;
    bufferwr[3]=0x00;
    bufferwr[4]=0x7F; //127
    CH347StreamI2C(serialnum,5,bufferwr,0,bufferrd); 

    //bufferwr[2]=0xA7;
    //CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd); 


    bufferwr[1]=0x40;
    //bufferwr[4]=0x00;

    
    bufferwr[16]=0x00;
    bufferwr[17]=0xC0;
    bufferwr[18]=0xF0;
    bufferwr[19]=0xFC;
    bufferwr[20]=0xFF;
    bufferwr[21]=0xFC;
    bufferwr[22]=0xF0;
    bufferwr[23]=0xC0;
    bufferwr[24]=0x00;

    bufferwr[1024]=0xFF;        
    result = CH347StreamI2C(serialnum,(128*8)+2,bufferwr,0,bufferrd); 
    if(!result){
        printf("Data send error.\n",result);
        return 0;        
    }
    bufferwr[1024]=0x00;        
    result = CH347StreamI2C(serialnum,(128*8)+2,bufferwr,0,bufferrd); 
    if(!result){
        printf("Data send error.\n",result);
        return 0;        
    }


    printf("\n");
    CH347CloseDevice(serialnum);
    return 0;
}