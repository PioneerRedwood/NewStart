// new_client_1.cpp -- 새출발 프로젝트
// WinSock, TCP/IP를 사용한 클라이언트 프로그래밍
// 2020.07.17 ~

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <WinSock2.h>

#pragma comment (lib, "ws2_32.lib")

#define BUFLEN 512
#define PORT "55999"

int main()
{
	// 소켓 초기화
	WSADATA wsaData;
	int iResult;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult == INVALID_SOCKET)
	{
		std::cout << ">> Error! WSAStartup() " << WSAGetLastError << "exit..\n";
		exit(EXIT_FAILURE);
	}

	// 통신 프로토콜 설정
	addrinfo* info = nullptr;
	addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// 서버 주소와 포트 설정
	iResult = getaddrinfo("127.0.0.1", PORT, &hints, &info);
	if (iResult != 0)
	{
		std::cout << ">> Error! registering info " << WSAGetLastError << "exit..\n";
		exit(EXIT_FAILURE);
	}

	// 연결할 소켓 생성
	SOCKET ConnectSocket = INVALID_SOCKET;
	ConnectSocket = socket(info->ai_family, info->ai_socktype, info->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		std::cout << ">> Error! creating socket " << WSAGetLastError << "exit..\n";
		freeaddrinfo(info);
		exit(EXIT_FAILURE);
	}

	// 사용자 정보 등록
	// 새로운 방식으로 전달

	// 3way-handshaking
	// 추가 예정

	// 서버에 연결
	iResult = connect(ConnectSocket, info->ai_addr, (int)info->ai_addrlen);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << ">> Error! connect() " << WSAGetLastError << "exit..\n";
		freeaddrinfo(info);
		exit(EXIT_FAILURE);
	}
	else 
	{
		// 접속한 서버 정보 보여주기
		char hostname[128];
		gethostname(hostname, 128);
		std::cout << ">> " << ConnectSocket << " connected to host: " << hostname
			<< " PORT: " << PORT << std::endl;
		std::cout << ">> insert \"quit\" if you want to quit\n";
		char buffer[BUFLEN];

		// 서버에 전송
		while (true)
		{
			// 접속을 끝내려면 콘솔에 quit 입력을 받음
			ZeroMemory(buffer, BUFLEN);
			std::cout << "# ";
			std::cin.getline(buffer, BUFLEN);
			
			if(strcmp(buffer, "quit"))
			{
				// 데이터 전송
				iResult = send(ConnectSocket, buffer, BUFLEN, 0);
				if (iResult == 0)
				{
					std::cout << ">> Error! send() " << WSAGetLastError << std::endl;
					freeaddrinfo(info);
					exit(EXIT_FAILURE);
				}
				else
				{
					std::cout << ">> sent " << iResult << " byte (s)\n";
					std::cin.clear();
				}
			}
			else
			{
				std::cout << ">> quit..\nexit";
				freeaddrinfo(info);
				break;
			}
		}
	}

	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << ">> Error! shutdown() " << WSAGetLastError << std::endl;
		freeaddrinfo(info);
		exit(EXIT_FAILURE);
	}

	closesocket(ConnectSocket);
	WSACleanup();
	return 0;
}