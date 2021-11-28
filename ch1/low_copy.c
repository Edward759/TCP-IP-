#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(char* message);

int main(void)
{
	int fd1, fd2;
	char buf[BUFSIZ];

	fd1=open("data.txt", O_RDONLY);
	if(fd1==-1)
		error_handling("open() error!");
	
	if(read(fd1, buf, sizeof(buf))==-1)
		error_handling("read() error!");

	fd2=open("data_copy.txt", O_CREAT|O_WRONLY|O_TRUNC);
	if(fd2==-1)
		error_handling("open() error!");

	char buf2[]
	if(write(fd2, buf, sizeof(buf))==-1)
		error_handling("write{} error!");

	printf("file data: %s", buf);

	close(fd1);
	close(fd2);

	return 0;
}

void error_handling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
