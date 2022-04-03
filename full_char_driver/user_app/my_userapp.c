#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<unistd.h>


char read_buf[1024];
char write_buf[1024];

#define PATH_OF_SYS_FILE "/dev/my_device"

int main() {


     int fd;
     char option;
     printf("************************Ritesh Mishra ***********\n");

     fd = open("/dev/my_device", O_RDWR);
     if(fd < 0) {
	     printf("Cant open file\n");
		return 0;
     }

     while(1) {
	printf("Enter option\n");
	printf("1. Write\n");
	printf("2. read\n");
	printf("3, exit\n");
	scanf("%c", &option);


	switch(option) {

	case '1' :
		printf("Enter the string to write into the buffer\n");
		scanf("%s", write_buf);
		printf("Data Writing...\n");
		write(fd, write_buf, strlen(write_buf)+1);
		printf("Done!\n");
		break;

	case '2' :
		printf("Data read back\n");
		read(fd, read_buf, 1024);
		printf("Done !!\n");
		printf("%s\n\n", read_buf);
		break;

	case '3' :
		close(fd);
		exit(1);
		break;
	
	default :
		printf("Enter the valid options\n");
	
	}

     }
     close(fd);



}
