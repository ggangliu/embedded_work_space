#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#define CHR_DEV_NAME "/dev/chr_dev"

int main(int argc, char** argv) {
	int ret = 0;
	char buf[100] = {0};
	char from_kernel_msg[] = "oooo, i come in.";
	int  fd = open(CHR_DEV_NAME, O_RDWR | O_NONBLOCK);
	if (fd < 0){
		printf("open file %s failed!\n", CHR_DEV_NAME);
		return -1;
	} 
	
	read(fd, buf, sizeof(buf)); // read from kernel 
	printf("buf = %s\n", buf);
	
	write(fd, from_kernel_msg, sizeof(from_kernel_msg)); //write to kernel

	close(fd);
	
	return 0;	
} 
