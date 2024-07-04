#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>

#include "blink_ioctl.h"

int main()
{
    int fd;
    printf("\nOpening Driver\n");
    fd = open("/dev/blinkled", O_RDWR);
    if(fd < 0) 
    {
        printf("Cannot open device file...\n");
        return 0;
    }
    ioctl(fd, IOCTL_BLINK); 
    close(fd);
}