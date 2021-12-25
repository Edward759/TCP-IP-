#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char* argv[])
{
	int serv_sock;
	int clnt_sock;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock==-1)
		error_handling("socket() error");

	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));

	if(bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr))==-1)
		error_handling("bind() error");

	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");

	clnt_addr_size=sizeof(clnt_addr);
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");

	int path_len;
	char path[1024];
	read(clnt_sock, (char*)(&path_len), 4);
	read(clnt_sock, path, path_len);

	int find=0;
	FILE*fp = fopen(path, "r");
	if(fp)
		find=1;
	
	write(clnt_sock, (char*)(&find),4);
	if(1 == find)
	{
		int read_cnt;
		char buf[BUF_SIZE];
		while(1)
		{
			read_cnt=fread((void*)buf, 1, BUF_SIZE, fp);
			if(read_cnt<BUF_SIZE)
			{
				write(clnt_sock, buf, read_cnt);
				break;
			}
			write(clnt_sock, buf, BUF_SIZE);
		}
		fclose(fp);
	}

	close(clnt_sock);
	close(serv_sock);
	return 0;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
