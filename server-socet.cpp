#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

int main()
{
	SetConsoleCP(1251);
	setlocale(LC_ALL, "Russian");
	WSADATA wsaData;
	ADDRINFO hints;
	ADDRINFO* addrResult;
	SOCKET ClientSocket = INVALID_SOCKET;
	SOCKET ListenSocket = INVALID_SOCKET;


	const char* sendBuffer = "Hello from erver";
	char recvBuffer[512];

	int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (result != 0) {
		cout << "WSAStartup failed" << endl;
		return 1;
	}
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	result = getaddrinfo(NULL, "788", &hints, &addrResult);
	if (result != 0) {
		cout << "getaddrinfo failed" << endl;
		return 1;
	}
	ListenSocket = socket(addrResult->ai_family, addrResult->ai_socktype, addrResult->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		cout << "socket creation with error" << endl;
		freeaddrinfo(addrResult);
		return 1;
	}

	result = bind(ListenSocket, addrResult->ai_addr, (int)addrResult->ai_addrlen);
	if (result == SOCKET_ERROR) {
		cout << "binding connect faild" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	result = listen(ListenSocket, SOMAXCONN);
	if (result == SOCKET_ERROR) {
		cout << "send Failed" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (result == SOCKET_ERROR) {
		cout << "Accepting faild" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	closesocket(ListenSocket);


	/*result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), MSG_DONTROUTE);
	if (result == SOCKET_ERROR) {
		cout << "send Failed" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	cout << "Sent " << result << " bytes";

	result = shutdown(ClientSocket, SD_SEND);
	if (result == SOCKET_ERROR) {
		cout << "shutdown error" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}*/
	do {
		ZeroMemory(recvBuffer, 512);
		result = recv(ClientSocket, recvBuffer, 512, 0);
		if (result > 0) {
			cout << "Получено" << result << "байт" << endl;
			cout << "Получено" << recvBuffer << endl;

			result = send(ClientSocket, sendBuffer, (int)strlen(sendBuffer), MSG_DONTROUTE);
			if (result == SOCKET_ERROR) {
				cout << "send Failed" << endl;
				freeaddrinfo(addrResult);
				WSACleanup();
				return 1;
			}
		}
		else if (result == 0) {
			cout << "Connection closed" << endl;
		}
		else {
			cout << "Сообщение не получено" << endl;
		}
	} while (result > 0);
	result = shutdown(ClientSocket, SD_SEND);
	if (result == SOCKET_ERROR) {
		cout << "shutdown error" << endl;
		freeaddrinfo(addrResult);
		WSACleanup();
		return 1;
	}
	closesocket(ClientSocket);
	freeaddrinfo(addrResult);
	WSACleanup();
	return 0;
}

