#pragma comment(lib, "CH347DLLA64.LIB")
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
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

    //clk divide ratio and osc freq at p35
    bufferwr[2]=0xD5;
    bufferwr[3]=0x80;
    CH347StreamI2C(serialnum,4,bufferwr,0,bufferrd);

    //Multiplex ratio default at p33
    bufferwr[2]=0xA8;
    bufferwr[3]=0x26;
    CH347StreamI2C(serialnum,4,bufferwr,0,bufferrd);

    //display offset p31
    bufferwr[2]=0xD3;
    bufferwr[3]=0x00;
    CH347StreamI2C(serialnum,4,bufferwr,0,bufferrd);

    //charge bump enable //p62
    bufferwr[2]=0x8D;
    bufferwr[3]=0x14;
    CH347StreamI2C(serialnum,4,bufferwr,0,bufferrd);    

    //memory addressing mode = Horizontal Addressing Mode p30,p34
    bufferwr[2]=0x20;
    bufferwr[3]=0x00;
    CH347StreamI2C(serialnum,4,bufferwr,0,bufferrd);  

    //column address //p36,p31
    bufferwr[2]=0xA1;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd); 


    //com remapped mode p31
    bufferwr[2]=0xC0;
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);

    //com pins p40,p31
    bufferwr[2]=0xDA;
    bufferwr[3]=0b00010010;    
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
    bufferwr[4]=0x03;
    CH347StreamI2C(serialnum,5,bufferwr,0,bufferrd);

    //Set Column Address 
    bufferwr[2]=0x21;
    bufferwr[3]=0;
    bufferwr[4]=127; 
    CH347StreamI2C(serialnum,5,bufferwr,0,bufferrd); 

    //bufferwr[2]=0xA7;
    //CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd); 

    Sleep(100);

    bufferwr[1]=0x40;
    //bufferwr[4]=0x00;

    const unsigned char mopurun_str[] = {
    0x7E,0x04,0x18,0x60,0x18,0x04,0x7E,0x00, //M
    0x00,0x3C,0x42,0x42,0x42,0x42,0x3C,0x00, //O
    0x00,0x7E,0x12,0x12,0x12,0x0C,0x00,0x00, //P
    0x00,0x3E,0x40,0x40,0x40,0x3E,0x00,0x00, //U
    0x00,0x7E,0x12,0x32,0x52,0x4C,0x00,0x00, //R
    0x00,0x3E,0x40,0x40,0x40,0x3E,0x00,0x00, //U
    0x00,0x7E,0x04,0x08,0x10,0x7E,0x00,0x00  //N
    };




    for(uint8_t i=0; i < sizeof(mopurun_str)/sizeof(mopurun_str[0]);i++){
        bufferwr[i+2] = mopurun_str[i];
    }


uint16_t tri=514*2;
for(uint16_t frame = 0 ; frame < 1000; frame++){

    //if(tri>=379){
        //bufferwr[tri-1]=0x0;        
        bufferwr[tri]=0xC0;
        bufferwr[tri+1]=0xF0;
        bufferwr[tri+2]=0xFC;
        bufferwr[tri+3]=0xFF;
        bufferwr[tri+4]=0xFC;
        bufferwr[tri+5]=0xF0;
        bufferwr[tri+6]=0xC0;
        bufferwr[tri+7]=0x0;
        bufferwr[tri+8]=0x0;
        bufferwr[tri+9]=0x0;
        bufferwr[tri+10]=0x0;    
        bufferwr[tri+11]=0x0;
        bufferwr[tri+12]=0x0;
        bufferwr[tri+13]=0x0;
        bufferwr[tri+14]=0x0;      
        for(uint8_t tri_white=0;tri_white<=8;tri_white++){
            bufferwr[tri_white+379-2]=0;
        }

        if(tri<=379){
            tri=514;
        }
        else{
            tri=tri-3;
        }
    //}


        //printf("Data send.\n");

    result = CH347StreamI2C(serialnum,(128*4)+2,bufferwr,0,bufferrd); 
    if(!result){
        printf("Data send error.\n",result);
        return 0;        
    }        

    //Sleep(10);    


}





       



    printf("\n");
    CH347CloseDevice(serialnum);
    return 0;
}