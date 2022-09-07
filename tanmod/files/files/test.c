#include<stdio.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

void main()
{
	char buf[10];
	printf("enter a string \n");
	gets(buf);
	printf("the entered string is %s \n",buf);
	char buffer[200];
	int fd;
	fd=open("/dev/my_device",O_RDWR);
	if(fd<0)
	{
		printf("cant open the file \n");
		
	}
	printf("writing into a buffer using chardev\n");
	write(fd,buf,strlen(buf));
	printf("reading from a buffer using chardev\n");
	read(fd,buffer,99);
	printf("%s \n", buffer);
	close(fd);
}


