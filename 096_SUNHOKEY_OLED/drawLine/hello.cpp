#pragma comment(lib, "CH347DLLA64.LIB")
#include <windows.h>
#include <stdio.h>
#include "CH347DLL.H"

unsigned long  serialnum = 0;
unsigned char bufferwr[4096] = {};
unsigned char bufferrd[4096] = {};


int coordinate_conv_left(int x, int y){
    int a=0;
    a = x+(y/8)*128+2;
    //printf("l:%d\n",a);  
    return a;    
}
int coordinate_conv_right(int y){
    int a=0;
    a = 1 << ((y+8) %(8*((y / 8)+1)));
    //printf("r:%d\n",a);    
    return a;
}
// x=0to127,y=0to63
void buffer_xyconv_or(int x, int y){
    bufferwr[coordinate_conv_left(x,y)] |=  coordinate_conv_right(y);
    return;
}

void buffer_drawLine(int x1,int y1,int x2,int y2){
    if(x1>x2){
        int a=x1;
        int b=y1;
        x1 = x2;
        y1 = y2;
        x2 = a;
        y2 = b;
    }
    int deltax = abs(x1-x2);
    int deltay = abs(y1-y2);
    float delta = (float)deltay/(float)deltax;
    float delta_rev = (float)deltax/(float)deltay;
    buffer_xyconv_or(x1,y1);
    buffer_xyconv_or(x2,y2);    
    if(delta>1){
        if(y1>y2){
            int a=x1;
            int b=y1;
            x1 = x2;
            y1 = y2;
            x2 = a;
            y2 = b;
        }
        if(x1<x2){
            for(int i=0;i<=deltay;i++){
                buffer_xyconv_or(x1+int(i*delta_rev), y1+i);
                //printf("%d.\n",(deltay));            
            }
        }
        else{
            for(int i=0;i<=deltay;i++){
                buffer_xyconv_or(x1-int(i*delta_rev), y1+i);            
            }
        }
    }
    else if(delta==1){
        if(y1<y2){
            for(int i=0;i<=deltax;i++){
                buffer_xyconv_or(x1+i, y1+i);            
            }
        }
        else{
            for(int i=0;i<=deltax;i++){
                buffer_xyconv_or(x1+i, y1-i);            
            }
        }

    }
    else if(delta<1){
        if(y1<y2){
            for(int i=0;i<=deltax;i++){
                buffer_xyconv_or(x1+i, y1+int(i*delta));
                //printf("%d.\n",(deltay));            
            }
        }
        else{
            for(int i=0;i<=deltax;i++){
                buffer_xyconv_or(x1+i, y1-int(i*delta));            
            }
        }
    }
    else{printf("drawLine Error\n");   }
    
    return;
}

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


/*


*/
    //reset the buffer
    bufferwr[1]=0x40;
    for(int i=2;i<=1025;i++){
        bufferwr[i]=0x00;
    }

    buffer_xyconv_or(0,0); 
    buffer_xyconv_or(0,63); 
    buffer_xyconv_or(127,0);    
    buffer_xyconv_or(127,63);
    buffer_drawLine(0,0,127,63) ;





    for(int i=0;i<=127;i+=5){
        buffer_drawLine(0,0,i,63) ;    
    }
    for(int i=0;i<=127;i+=5){
        buffer_drawLine(127,0,i,63) ;    
    }





    buffer_drawLine(0,0,63,63) ;
    buffer_drawLine(63,0,0,63) ;
    buffer_drawLine(64,0,127,63) ;  
    buffer_drawLine(64,63,127,0) ;         
    bufferwr[1]=0x40;
    //bufferwr[4]=0x00;

    




    result = CH347StreamI2C(serialnum,(128*8)+2,bufferwr,0,bufferrd); 
    if(!result){
        printf("Data send error.\n",result);
        return 0;        
    }
        
    result = CH347StreamI2C(serialnum,(128*8)+2,bufferwr,0,bufferrd); 
    if(!result){
        printf("Data send error.\n",result);
        return 0;        
    }


    printf("\n");
    CH347CloseDevice(serialnum);
    return 0;
}
//eight pages
//2to129
//130to257
//258to385
//386to513
//514to641
//642to769
//770to897
//898to1025

