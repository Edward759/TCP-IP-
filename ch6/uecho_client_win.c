#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 1024
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	char send_msg[BUF_SIZE];
	char recv_msg[BUF_SIZE];
	int strLen = 0;

	if (argc != 3)
	{
		printf("Usage : %s <IP> <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_DGRAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("UDP socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	while (1)
	{
		fputs("Insert message(q to quit): ", stdout);
		fgets(send_msg, sizeof(send_msg), stdin);
		if (!strcmp(send_msg, "q\n") || !strcmp(send_msg, "Q\n"))
			break;

		send(hSocket, send_msg, strlen(send_msg), 0);

		strLen = recv(hSocket, recv_msg, BUF_SIZE - 1, 0);
		recv_msg[strLen] = 0;
		printf("Message from server: %s", recv_msg);
	}

	closesocket(hSocket);
	WSACleanup();
	return 0;
}

void ErrorHandling(char* message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}