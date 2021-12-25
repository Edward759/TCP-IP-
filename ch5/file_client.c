#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 39
void error_handling(char *message);

int main(int argc, char* argv[])
{
	int sock;
	struct sockaddr_in serv_addr;
	
	if(argc!=4)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock=socket(PF_INET, SOCK_STREAM, 0);
	if(sock==-1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_addr.sin_port=htons(atoi(argv[2]));

	if(connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("connect() error");

	FILE* fp=fopen("receive.txt", "wb"); 
	int path_len = strlen(argv[3]) + 1;
	write(sock, (char*)(&path_len), 4);
	write(sock, argv[3], path_len);

	int find;
	read(sock, (char*)(&find), 4);
	char buf[BUF_SIZE];
	int read_cnt;

	if(0==find)
		fputs("file not found\n", stdout);
	else
	{
		while((read_cnt=read(sock, buf, BUF_SIZE))!=0)
			fwrite((void*)buf, 1, read_cnt, fp);	
	
		puts("Received file fata");
	}
	fclose(fp);
	close(sock);
	return 0;
}

void error_handling(char *message)
{
        fputs(message, stderr);
        fputc('\n', stderr);
        exit(1);
}
