// new_server_1.cpp -- ����� ������Ʈ
// WinSock, TCP/IP�� ����� ���� ���α׷���
// 2020.07.17 ~

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <sstream>
#include <string>

#pragma comment (lib, "ws2_32.lib")

#define BUFLEN 512
#define PORT "55999"
#define BACKLOG 10

int main()
{
	// ���� �ʱ�ȭ
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << ">>Error! WSAStartup()" << WSAGetLastError << " exit..\n";
		exit(EXIT_FAILURE);
	}

	// ��� �������� ����
	addrinfo* info = nullptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// ���� �ּ� �� ��Ʈ ���
	iResult = getaddrinfo(nullptr, PORT, &hints, &info);
	if (iResult != 0)
	{
		std::cout << ">> Error! registering info" << WSAGetLastError << " exit..\n";
		exit(EXIT_FAILURE);
	}

	// Ŭ���̾�Ʈ ���� ���� ����
	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		std::cout << ">> Error! creating socket" << WSAGetLastError << " exit..\n";
		freeaddrinfo(info);
		exit(EXIT_FAILURE);
	}

	// TCP ��� ���� ���ε�
	iResult = bind(ListenSocket, info->ai_addr, (int)info->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << ">> Error! bind()" << WSAGetLastError << " exit..\n";
		freeaddrinfo(info);
		exit(EXIT_FAILURE);
	}

	// listen ����
	if (listen(ListenSocket, BACKLOG) == SOCKET_ERROR)
	{
		std::cout << ">> Error! listen()" << WSAGetLastError << " exit..\n";
		freeaddrinfo(info);
		exit(EXIT_FAILURE);
	}

	std::cout << ">> Waiting for connection.. \n";

	// ���� ��ũ���� ����
	FD_SET master;

	FD_ZERO(&master);
	FD_SET(ListenSocket, &master);

	// 3way-handshaking ��ġ ���� ��


	// ��� ����
	while (true)
	{
		FD_SET sockets = master;

		unsigned socketCount = select(sockets.fd_count + 1, &sockets, nullptr, nullptr, nullptr);
		

		if (socketCount == SOCKET_ERROR)
		{
			std::cout << ">> Error! select() " << WSAGetLastError << " exit..\n";
			freeaddrinfo(info);
			exit(EXIT_FAILURE);
		}
		else if (socketCount == 0)
		{
			std::cout << ">> select(): 0\n";
			continue;
		}
		else
		{
			// FD_SET�� ���ӵ� ��� ������ �˻��ϱ� ������ ���� ���� �������� ����ȭ�� �����
			for (unsigned i = 0; i < socketCount; i++)
			{
				// ���� ����
				SOCKET nowsock = sockets.fd_array[i];

				// ���� �������� ���� ���ο� ���� FD_SET�� �߰�
				if (ListenSocket == nowsock)
				{
					// ���� ������ �� ����
					std::cout << ">> connenctions: " << master.fd_count - 1 << std::endl;

					// ���� ��� ����
					sockaddr_in client;
					int szClient = sizeof(client);
					SOCKET socket = accept(ListenSocket, (sockaddr*)&client, &szClient);
					if (socket == INVALID_SOCKET)
					{
						std::cout << ">> Error! accept()" << WSAGetLastError << " exit..\n";
						freeaddrinfo(info);
						exit(EXIT_FAILURE);
					}

					FD_SET(socket, &master);
					
					std::string welcomeMsg = ">> Welcome! connected to server!\r\n";
					send(socket, welcomeMsg.c_str(), welcomeMsg.size() + 1, 0);


					// Ŭ���̾�Ʈ ���� ���� ����
					char host[NI_MAXHOST];
					char service[NI_MAXSERV];

					ZeroMemory(host, NI_MAXHOST);
					ZeroMemory(service, NI_MAXSERV);

					if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST,
						service, NI_MAXSERV, 0) == 0)
					{
						std::cout << host << " is connected to " << service << std::endl;
					}
					else
					{
						inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
						std::cout << host << "is connected to " << service << std::endl;
					}
				}
				// ���� ������ �ƴ� �ٸ� �������κ����� �̺�Ʈ �߻�
				else
				{
					// ���� ����
					char buffer[BUFLEN];
					ZeroMemory(buffer, BUFLEN);
					iResult = recv(nowsock, buffer, BUFLEN, 0);
					
					std::cout << nowsock << ": " << buffer << std::endl;

					// ���� Ȥ�� "quit"�̸� ����
					if (iResult <= 0)
					{
						if (strcmp(buffer, "quit") == 0)
						{
							std::cout << ">> quit exit..\n";
						}
						else
						{
							std::cout << ">> Disconnected to " << nowsock << std::endl;
						}
						FD_CLR(nowsock, &master);
						closesocket(nowsock);
					}
					else
					{
						// ��ε�ĳ����
						// ���� �� ���� ���� ���Ͽ� ����
						for (unsigned i = 0; i < master.fd_count; i++)
						{
							SOCKET outsock = master.fd_array[i];
							if (outsock != ListenSocket && outsock != nowsock)
							{
								std::ostringstream ss;
								ss << outsock << ": " << buffer << "\r";
								std::string strOut = ss.str();

								send(outsock, strOut.c_str(), strOut.size() + 1, 0);
								ss.clear();
							}
						}
					}
				}
			}
		}
	}

	// ����
	iResult = shutdown(ListenSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << ">> Error! shutdown() " << WSAGetLastError << std::endl;
		freeaddrinfo(info);
		exit(EXIT_FAILURE);
	}

	closesocket(ListenSocket);
	WSACleanup();
	return 0;
}

void _3way_handshaking()
{

}