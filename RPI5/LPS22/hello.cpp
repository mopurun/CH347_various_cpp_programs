#pragma comment(lib,"libch347.a")
#include <stdint.h>
#include <cstdio>
#include "ch347_lib.h"

int result = 0;
int main(int argc , char *argv[]){
	result = CH347OpenDevice(argv[1]);
	if(result==-1){
		printf("Connection failed.\n");
		return -1;
	}
	//printf("OpenDeviceResult:%d\n",result);
	int fd = result;
	CH347I2C_Set(fd,2);
	unsigned char bufferwr[10];
	unsigned char bufferrd[10];
	bufferwr[0]=0xBA;
	bufferwr[1]=0x0F;
	CH347StreamI2C(fd,2,bufferwr,1,bufferrd);
	printf("%02x\n",bufferrd[0]);
	CH347CloseDevice(fd);
	
	
	
	
	return 0;
}
