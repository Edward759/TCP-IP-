#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hSocket;
	SOCKADDR_IN servAddr;

	if (argc != 4)
	{
		printf("Usage : %s <IP> <port> <path>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = inet_addr(argv[1]);
	servAddr.sin_port = htons(atoi(argv[2]));

	if (connect(hSocket, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("connect() error!");

	int strLen = strlen(argv[3]) + 1;
	send(hSocket, (char*)(&strLen), 4, 0);
	send(hSocket, argv[3], strLen, 0);

	int find;
	if (recv(hSocket, (char*)(&find), sizeof(int), 0) == -1)
		ErrorHandling("recv() error!");

	if (1 != find)
	{
		fputs("file not found!", stdout);
	}
	else
	{
		char buf[BUF_SIZE];
		int readCnt;
		FILE* fp = fopen("receive.txt", "wb");
		while ((readCnt = recv(hSocket, buf, BUF_SIZE, 0)) != 0)
			fwrite((void*)buf, 1, readCnt, fp);

		fputs("Received file data!", stdout);
		fclose(fp);
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