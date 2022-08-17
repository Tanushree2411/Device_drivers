#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

void main()
{
	char buf[10]="Hello";
	char buffer[200];
	int fd;
	fd=open("/dev/chardev",O_RDWR);
	printf("writing into a buffer using chardev\n");
	write(fd,buf,strlen(buf));
	printf("reading from a buffer using chardev\n");
	read(fd,buffer,99);
	printf("%s", buffer);
}


