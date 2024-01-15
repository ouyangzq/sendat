#include <termios.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "openDev.h"

#define TRUE 1
#define FALSE 0
 
int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300, B38400, B19200, B9600, B4800, B2400, B1200, B300, };
int name_arr[] = {38400, 19200, 9600, 4800, 2400, 1200, 300, 38400, 19200, 9600, 4800, 2400, 1200, 300, };
 
int OpenDev(char *Dev)
{
 // , O_RDWR|O_NOCTTY
  int fd = open(Dev,O_RDWR | O_NOCTTY | O_NONBLOCK);
  if(-1 == fd)
    {
      perror("Can't Open Serial PPPPort");
      return -1;
    } 
  else 
    {
      return fd;
    }
} 

void set_speed(int fd, int speed)
{ 
  int i; 
  int status; 
  struct termios Opt;
 tcgetattr(fd, &Opt); 
  for ( i= 0; i < sizeof(speed_arr) / sizeof(int); i++)
   { 
      if (speed == name_arr[i]) 
        { 
          tcflush(fd, TCIOFLUSH); 
          cfsetispeed(&Opt, speed_arr[i]);
         cfsetospeed(&Opt, speed_arr[i]);
         status = tcsetattr(fd, TCSANOW, &Opt); 
          if (status != 0) perror("tcsetattr fd1");
         return;
       } 
    tcflush(fd,TCIOFLUSH);
    }
}

int set_Parity(int fd,int databits,int stopbits,int parity) 
{ 
  struct termios options; 
  if ( tcgetattr( fd,&options) != 0) 
  {
   perror("SetupSerial 1");
   return(FALSE);
 } 
  bzero(&options,sizeof(options)); 
  options.c_cflag |= CLOCAL | CREAD;
 options.c_cflag &= ~CSIZE; 
  switch (databits) 
  { 
    case 7: 
    options.c_cflag |= CS7;
   break;
   case 8:
   options.c_cflag |= CS8;
   break; 
    default: fprintf(stderr,"Unsupported data size\n");
   return (FALSE); 
  } 
  switch (parity) 
  {
   case 'n': 
    case 'N':
   options.c_cflag &= ~PARENB;
   options.c_iflag &= ~INPCK; 
    break; 
    case 'o':
   case 'O': 
    options.c_cflag |= (PARODD | PARENB);
   options.c_iflag |= (INPCK | ISTRIP); 
    break; 
    case 'e': 
    case 'E': 
    options.c_cflag |= PARENB;
   options.c_cflag &= ~PARODD; 
    options.c_iflag |= (INPCK | ISTRIP); 
    break; 
    case 'S': 
    case 's': 
    options.c_cflag &= ~PARENB; 
    options.c_cflag &= ~CSTOPB;
   break;
   default: fprintf(stderr,"Unsupported parity\n"); 
   return (FALSE); 
  } 
  switch (stopbits)
 { 
    case 1:
   options.c_cflag &= ~CSTOPB; 
    break; 
    case 2: 
    options.c_cflag |= CSTOPB;
   break; 
    default: fprintf(stderr,"Unsupported stop bits\n"); 
    return (FALSE); 
    } 
    if (parity != 'n') 
    options.c_iflag |= INPCK; 
    options.c_cc[VTIME] = 0;
   options.c_cc[VMIN] = 0;
   tcflush(fd,TCIFLUSH); 
    if (tcsetattr(fd,TCSANOW,&options) != 0)
   {  
        perror("SetupSerial 3"); 
        return (FALSE);
   } 
    return (TRUE);
}
