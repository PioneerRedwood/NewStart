// new_server_1.cpp -- 새출발 프로젝트
// WinSock, TCP/IP를 사용한 서버 프로그래밍
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
	// 소켓 초기화
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << ">>Error! WSAStartup()" << WSAGetLastError << " exit..\n";
		exit(EXIT_FAILURE);
	}

	// 통신 프로토콜 설정
	addrinfo* info = nullptr, hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// 서버 주소 및 포트 등록
	iResult = getaddrinfo(nullptr, PORT, &hints, &info);
	if (iResult != 0)
	{
		std::cout << ">> Error! registering info" << WSAGetLastError << " exit..\n";
		exit(EXIT_FAILURE);
	}

	// 클라이언트 접속 소켓 생성
	SOCKET ListenSocket = INVALID_SOCKET;
	ListenSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (ListenSocket == INVALID_SOCKET)
	{
		std::cout << ">> Error! creating socket" << WSAGetLastError << " exit..\n";
		freeaddrinfo(info);
		exit(EXIT_FAILURE);
	}

	// TCP 통신 소켓 바인드
	iResult = bind(ListenSocket, info->ai_addr, (int)info->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << ">> Error! bind()" << WSAGetLastError << " exit..\n";
		freeaddrinfo(info);
		exit(EXIT_FAILURE);
	}

	// listen 실행
	if (listen(ListenSocket, BACKLOG) == SOCKET_ERROR)
	{
		std::cout << ">> Error! listen()" << WSAGetLastError << " exit..\n";
		freeaddrinfo(info);
		exit(EXIT_FAILURE);
	}

	std::cout << ">> Waiting for connection.. \n";

	// 파일 디스크립터 생성
	FD_SET master;

	FD_ZERO(&master);
	FD_SET(ListenSocket, &master);

	// 3way-handshaking 위치 생각 중


	// 통신 진행
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
			// FD_SET에 접속된 모든 소켓을 검사하기 때문에 접속 수가 많아지면 과부화가 예상됨
			for (unsigned i = 0; i < socketCount; i++)
			{
				// 현재 소켓
				SOCKET nowsock = sockets.fd_array[i];

				// 서버 소켓으로 오면 새로운 소켓 FD_SET에 추가
				if (ListenSocket == nowsock)
				{
					// 현재 접속자 수 전시
					std::cout << ">> connenctions: " << master.fd_count - 1 << std::endl;

					// 접속 허용 설정
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


					// 클라이언트 접속 내용 전시
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
				// 서버 소켓이 아닌 다른 소켓으로부터의 이벤트 발생
				else
				{
					// 받은 내용
					char buffer[BUFLEN];
					ZeroMemory(buffer, BUFLEN);
					iResult = recv(nowsock, buffer, BUFLEN, 0);
					
					std::cout << nowsock << ": " << buffer << std::endl;

					// 오류 혹은 "quit"이면 종료
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
						// 브로드캐스팅
						// 본인 및 서버 제외 소켓에 전송
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

	// 종료
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