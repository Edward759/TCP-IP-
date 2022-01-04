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
	int sock;
	char send_msg[BUF_SIZE];
	char recv_msg[] = "from client";
	int str_len;
	socklen_t adr_sz;

	struct sockaddr_in serv_adr, from_adr;

	if(argc!=3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	sock = socket(PF_INET, SOCK_DGRAM, 0);
	if(sock == -1)
		error_handling("socket() error");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=inet_addr(argv[1]);
	serv_adr.sin_port=htons(atoi(argv[2]));

	while(1)
	{
		fputs("Insert message(q to quit): ", stdout);
		fgets(send_msg, sizeof(send_msg), stdin);
		if(!strcmp(send_msg, "q\n") || !strcmp(send_msg, "Q\n"))
			break;
		
		sendto(sock, send_msg, sizeof(send_msg), 0, (struct sockaddr*)&serv_adr, sizeof(serv_adr));

		adr_sz = sizeof(from_adr);
		str_len = recvfrom(sock, recv_msg, BUF_SIZE, 0, (struct sockaddr*)&from_adr, &adr_sz);
		recv_msg[str_len] = 0;
		printf("Message from server: %s", recv_msg);
	}

	close(sock);
	return 0;
}


void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}
