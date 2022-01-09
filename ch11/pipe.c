#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[])
{
	int fds1[2], fds2[2];
	char str[BUF_SIZE];
	char buf[BUF_SIZE];

	pipe(fds1);
	pipe(fds2);

	pid_t pid = fork();

	for(int i=0; i<3; i++)
	{
		if(pid==0)
		
		{
			fgets(str, BUF_SIZE, stdin);
			write(fds1[1], str, sizeof(str));
			read(fds2[0], buf, BUF_SIZE);
			printf("child output:%s", buf);
		}
		else
		{
			read(fds1[0], buf, BUF_SIZE);
			printf("parent output:%s", buf);
			fgets(str, BUF_SIZE, stdin);
			write(fds2[1], str, sizeof(str));
		}
	}
	return 0;
}
