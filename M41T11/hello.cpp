#pragma comment(lib, "CH347DLLA64.LIB")
#include <windows.h>
#include <stdio.h>
#include "CH347DLL.H"

unsigned long  serialnum = 0;
unsigned char bufferwr[4096];
unsigned char bufferrd[4096];


char* conv_num2day(unsigned char num){

    if(num==1){
        return "Sun";
    }
    else if(num==2){
        return "Mon";
    }
    else if(num==3){
        return "Tue";
    }
    else if(num==4){
        return "Wed";
    }
    else if(num==5){
        return "Thu";
    }
    else if(num==6){
        return "Fri";
    }
    else if(num==7){
        return "Sat";
    }
    else{return "ERR";}
    return "ERR";
}

int main(void)
{
    CH347OpenDevice(serialnum);
    
    bool result = CH347I2C_Set(serialnum,2);
    if(!result){
        printf("Device not found.\n",result);
        return 0;        
    }   

    //write to 0x07
    bufferwr[0]=0xD0;
    bufferwr[1]=0x07;
    bufferwr[2]=0x80;    
    CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);       

    //set time
    //bufferwr[1]=0x02;
    //bufferwr[2]=0x13;    
    //CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);

    unsigned char osec_tmp = 0;

    //40sec
    unsigned int i ;
    for(i=0;i<400;i++){


        bufferwr[1]=0x00;
        result = CH347StreamI2C(serialnum,2,bufferwr,8,bufferrd); 
        unsigned char osec = bufferrd[0] & 0xF  ;
        unsigned char tsec = (bufferrd[0] >> 4) & 0x7 ;
        unsigned char omin = bufferrd[1] & 0xF  ;
        unsigned char tmin = (bufferrd[1] >> 4) & 0x7;
        unsigned char ohour = bufferrd[2] & 0xF;
        unsigned char thour = (bufferrd[2] >> 4) & 0x3 ;
        char* day = conv_num2day(bufferrd[3] & 0x7);
        unsigned char odate = bufferrd[4] & 0xF  ;
        unsigned char tdate = (bufferrd[4] >> 4) & 0x3 ;
        unsigned char omon = bufferrd[5] & 0xF  ;
        unsigned char tmon = (bufferrd[5] >> 4) & 0x1 ;
        unsigned char oyear = bufferrd[6] & 0xF  ;
        unsigned char tyear = (bufferrd[6] >> 4) ;      
        if(i==0){
            printf("ST:%d,CEB:%d,CB:%d,OUT:%d,FT:%d,S:%d\n",(bufferrd[0] >> 7) & 0x1,(bufferrd[2] >> 7) & 0x1,(bufferrd[2] >> 6) & 0x1,(bufferrd[7] >> 7) & 0x1,(bufferrd[7] >> 6) & 0x1,(bufferrd[7] >> 5) & 0x1);

        }  

        if(osec != osec_tmp){
            bufferwr[1]=0x07;
            bufferwr[2]=0x80;    
            CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);
        }
        
        Sleep(100);

        printf("%d%d/%d%d/%d%d",tyear,oyear,tmon,omon,tdate,odate);
        printf("(%s)",day);
        printf("%d%d:%d%d:%d%d\r",thour,ohour,tmin,omin,tsec,osec);
        ///printf("PRESS_OUT:%dhPa:%d:%d\r",bufferrd[0],bufferrd[1],PRESS_OUT_XL);
        
        if(!result){
            printf("\nConnection failed.");
            return 0;
        }


        if(osec != osec_tmp){
            bufferwr[1]=0x07;
            bufferwr[2]=0x00;    
            CH347StreamI2C(serialnum,3,bufferwr,0,bufferrd);
        }
        osec_tmp = osec;


    }
    printf("\n");
    CH347CloseDevice(serialnum);
    return 0;
}