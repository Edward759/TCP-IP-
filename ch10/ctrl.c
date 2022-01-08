#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void keycontrol(int sig)
{
	if(sig==SIGINT)
	{
		char msg[10];
		puts("exit? Y or N");
		fgets(msg, 10, stdin);

		if(!strcmp(msg, "y\n") || !strcmp(msg, "Y\n"))
			exit(1);
		else
			return;
	}
}

int main(int argc, char* argv[])
{
	int i;
	signal(SIGINT, keycontrol);

	while(1)
	{
		sleep(1);
		puts("wait...");
	}

	return 0;
}
