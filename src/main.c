#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <assert.h> 
#include <sys/time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <termios.h>
#include <stdarg.h>
#include <sys/select.h>
#include <ctype.h>
#include <errno.h>
#include <getopt.h>
#include <signal.h>
#include "openDev.h"
#define MAX_PORTS	4
/*this array hold information about each port we have opened */
struct PortInfo {
	char name[16];
	int  port_fd;
}
;
/*
** File: uart.c
**
** Description:
** Provides an RS-232 interface that is very similar to the CVI provided
** interface library
*/
/*this array hold information about each port we have opened */
struct PortInfo ports[13] = { {
		"/dev/ttyUSB0", 0
	}
	, {
		"/dev/ttyUSB1", 0
	}
	, {
		"/dev/ttyUSB2", 0
	}
	, {
		"/dev/ttyUSB3", 0
	}
	, {
		"/dev/ttyUSB4", 0
	}
	, {
		"/dev/ttyUSB5", 0
	}
	, {
		"/dev/ttyUSB6", 0
	}
	, {
		"/dev/ttyUSB7", 0
	}
	, {
		"/dev/ttyUSB8", 0
	}
	, {
		"/dev/ttyUSB9", 0
	}
	, {
		"/dev/ttyUSB10", 0
	}
	, {
		"/dev/ttyUSB11", 0
	}
	, {
		"/dev/ttyUSB12", 0
	}
	,
};
/*判断是否存在*/
int FileExist(const char* filename) {
	if (filename && access(filename, F_OK) == 0) {
		return 1;
	}
	return 0;
}
int main(int argc, char **argv) {
	if(argc<3) {
		printf("ERROR demo: sendat 2 'ATI'\n");
		exit(1);
		return 0;
	}
	int debug = 1;
	int port= 0;
	sscanf(argv[1], "%d", &port);
	if(FileExist(ports[port].name)==0) {
		printf("AT ERROR absent.\n");
		return 0;
	}
	char *message= argv[2];
	char *nty= "\r\n";
	int fd = OpenDev(ports[port].name);
	//打开串口 
	if(fd>0) {
		set_speed(fd,152000);
		//设置波特率
	} else {
		printf("Can't Open Serial Port!\n");
		exit(1);
		return 0;
	}
    //设置校验位
	if(set_Parity(fd,8,1,'N')==FALSE)  {
		printf("Set Parity Error\n");
		exit(1);
	}
	int nread;
	char buffer[MAX_BUFF_SIZE];
	serial_parse phandle;
	phandle.rxbuffsize = 0;
	char *ATcStr = strcat(message, nty);
	int ATlen = strlen(ATcStr);
	char sendAT[ATlen + 1];
	strcpy(sendAT, ATcStr);
	write(fd, sendAT, strlen(sendAT));
	usleep(10000);
	nread = read(fd , phandle.buff + phandle.rxbuffsize, MAX_BUFF_SIZE - phandle.rxbuffsize);
	phandle.rxbuffsize += nread;
	phandle.buff[phandle.rxbuffsize] = '\0';
	printf("%s",phandle.buff);
	exit(1);
	return 0;
}
