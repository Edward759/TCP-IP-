#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
	pid_t pid;
	int fd = open("test.txt", O_CREAT);

	pid = fork();
	if(pid==0)
		printf("child:%d\n", fd);
	else
		printf("parent:%d\n", fd);

	return 0;
}
