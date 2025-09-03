#pragma comment(lib, "CH347DLLA64.LIB")
#include <windows.h>
#include <stdio.h>
#include <stdint.h>
#include "CH347DLL.H"

uint8_t gpio_data =0x0;
unsigned long  serialnum = 0;
uint8_t play_num =0;
uint32_t play_time_lst[12]={8460,19400,13680,21030,18140,9780,15560,12170,17630,20180,5890,12990};


void send_data(uint8_t play_num = 0){
            //printf("data_");
    for(int i=3;i>=0;--i){
        if((play_num>>i)&1u){
            gpio_data = 1;
            CH347GPIO_Set(serialnum,0x07,0x07,gpio_data); 
            gpio_data = 3;
            CH347GPIO_Set(serialnum,0x07,0x07,gpio_data);  
            gpio_data = 2;
            CH347GPIO_Set(serialnum,0x07,0x07,gpio_data); 
            gpio_data = 0;
            CH347GPIO_Set(serialnum,0x07,0x07,gpio_data);
            //printf("1");                        
        }
        else{
            gpio_data = 0;
            CH347GPIO_Set(serialnum,0x07,0x07,gpio_data); 
            gpio_data = 2;
            CH347GPIO_Set(serialnum,0x07,0x07,gpio_data);  
            gpio_data = 2;
            CH347GPIO_Set(serialnum,0x07,0x07,gpio_data); 
            gpio_data = 0;
            CH347GPIO_Set(serialnum,0x07,0x07,gpio_data); 
            //printf("0");            
        }        
    }
            //printf("\n");    

}

void sound_play(uint8_t play_num,uint32_t play_time = 0){
    if(play_time==0){
        play_time=play_time_lst[play_num-1];
    }
    send_data(play_num-1);
    printf("Now playing... No.%d\n",play_num);
    gpio_data = 4;
    CH347GPIO_Set(serialnum,0x07,0x07,gpio_data);    
    Sleep(play_time);
    gpio_data = 0;
    CH347GPIO_Set(serialnum,0x07,0x07,gpio_data);     
}

int main(void)
{

    CH347OpenDevice(serialnum);
    CH347Uart_Open(serialnum);

    

    sound_play(1);
    sound_play(2);
    sound_play(3);
    sound_play(4);
    sound_play(5);
    sound_play(6);
    sound_play(7);
    sound_play(8);
    sound_play(9);
    sound_play(10);
    sound_play(11);
    sound_play(12);

    printf("Close\n");
    CH347Uart_Close(serialnum);
    CH347CloseDevice(serialnum);
    return 0;
}


