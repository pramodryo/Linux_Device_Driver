#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/ioctl.h>


#define WR_VALUE _IOW('a','b',int32_t*)
#define RD_VALUE _IOR('a','b',int32_t*)


int8_t write_buf[1024];
int8_t read_buf[1024];

int main()
{
    int fd;
    char option;
    int32_t ioctl_num;
    printf("*************** WEL-COME ******************\n");

    fd = open("/dev/etx_device", O_RDWR);
    if(fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
    }

    while(1) {
        printf("****Please Enter the Option******\n");
        printf("        1. Write string         \n");
        printf("        2. Read string          \n");
        printf("        3. Exit                 \n");
        printf("        4. Write In ioctl       \n");
        printf("        5. read from ioctl       \n");
        printf("*********************************\n");
        scanf(" %c", &option);
        printf("Your Option = %c\n", option);

        switch(option) {
            case '1':
                {
                    printf("Enter the string to write into driver: ");
                    scanf("  %[^\t\n]s", write_buf);
                    printf("Data Writing ...");
                    write(fd, write_buf, strlen(write_buf)+1);
                    printf("Done!\n");
                }break;
            case '2':
                {
                    printf("Data Reading ...");
                    read(fd, read_buf, 1024);
                    printf("Done!\n\n");
                    printf("Data = %s\n\n", read_buf);
                }break;
            case '3':
                {
                    close(fd);
                    exit(1);
                }break;
            case '4':
                {
                    printf("Enter the value to be write in ioctl\n");
                    scanf(" %d", &ioctl_num);
                    ioctl(fd, WR_VALUE, (int32_t*)&ioctl_num);
                }break;
            case '5':
                {
                    printf("reading value from drive via ioctl\n");
                    ioctl(fd, RD_VALUE, (int32_t*) &ioctl_num);
                    printf("ioctl value is %d\n", ioctl_num);
                }break;

            default:
                printf("Enter Valid option = %c\n",option);
                break;
        }
    }
    close(fd);
}

