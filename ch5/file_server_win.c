#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#define BUF_SIZE 30
void ErrorHandling(char* message);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	SOCKET hServSock, hClntSock;
	SOCKADDR_IN servAddr, clntAddr;

	if (argc != 2)
	{
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		ErrorHandling("WSAStartup() error!");

	hServSock = socket(PF_INET, SOCK_STREAM, 0);
	if (hServSock == INVALID_SOCKET)
		ErrorHandling("socket() error");

	memset(&servAddr, 0, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servAddr.sin_port = htons(atoi(argv[1]));

	if (bind(hServSock, (SOCKADDR*)&servAddr, sizeof(servAddr)) == SOCKET_ERROR)
		ErrorHandling("bind() error");

	if (listen(hServSock, 5) == SOCKET_ERROR)
		ErrorHandling("listen() error");

	int szClntAddr;
	szClntAddr = sizeof(clntAddr);
	hClntSock = accept(hServSock, (SOCKADDR*)&clntAddr, &szClntAddr);
	if (hClntSock == INVALID_SOCKET)
		ErrorHandling("accept() error");

	
	int readLen, find = 0;
	recv(hClntSock, (char*)(&readLen), 4, 0);

	char path[1024];
	recv(hClntSock, path, readLen, 0);

	fputs(path, stdout);
	FILE* fp = fopen(path, "r");
	if (fp)
		find = 1;

	if(send(hClntSock, (char*)(&find), sizeof(int), 0) == -1)
		ErrorHandling("send() error!");

	Sleep(1000);
	if (1 == find)
	{
		char buf[BUF_SIZE];
		int readCnt;
		while (1)
		{
			readCnt = fread((void*)buf, 1, BUF_SIZE, fp);
			if (readCnt < BUF_SIZE)
			{
				send(hClntSock, (char*)&buf, readCnt, 0);
				break;
			}
			send(hClntSock, (char*)&buf, BUF_SIZE, 0);
		}
		fclose(fp);
	}
	closesocket(hClntSock);
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