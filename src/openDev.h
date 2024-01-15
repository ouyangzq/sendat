#ifndef OPENDEV_H
#define OPENDEV_H

#include <math.h>

#define TRUE 1
#define FALSE 0
#define MAX_BUFF_SIZE (256)

typedef struct _serial_parse
{
    char buff[MAX_BUFF_SIZE];
    int rxbuffsize;
}serial_parse;

int OpenDev(char *Dev);
void set_speed(int fd, int speed);
int set_Parity(int fd,int databits,int stopbits,int parity);

#endif
