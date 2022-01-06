#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024

void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock;
	char recv_msg[BUF_SIZE];
	char send_msg[BUF_SIZE];

	SOCKADDR_IN servAddr, clntAddr;
	int clntAdrSz;
	int strLen;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_DGRAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("UDP socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	while (1)
	{
		clntAdrSz = sizeof(clntAddr);
		strLen = recvfrom(hServSock, recv_msg, BUF_SIZE, 0, (SOCKADDR*)&clntAddr, &clntAdrSz);
		recv_msg[strLen] = 0;
		printf("Message from client: %s", recv_msg);

		fputs("Insert message(q to quit): ", stdout);
		fgets(send_msg, sizeof(send_msg), stdin);
		if (!strcmp(send_msg, "q\n") || !strcmp(send_msg, "Q\n"))
			break;
		sendto(hServSock, send_msg, sizeof(send_msg) - 1, 0, (struct sockaddr*)&clntAddr, clntAdrSz);
	}

	closesocket(hServSock);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}