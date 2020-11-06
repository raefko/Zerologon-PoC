#include "network.h"
#include <string>
#pragma comment(lib,"ws2_32.lib") //Winsock Library

SOCKET tcpConnect()
{
	WSADATA wsa;
	sockaddr_in hint;
	SOCKET socket_;

	std::string ipAddress = "127.0.0.1";
	int port = 9996;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != ERROR_SUCCESS)
	{
		std::cerr << "Failed. Error Code : " << WSAGetLastError() << "\n";
		return NULL;
	}
	if ((socket_ = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		std::cerr << "Could not create socket : " << WSAGetLastError() << "\n";
		return NULL;
	}
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	if (connect(socket_, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
	{
		std::cerr << "Can't connect to server, Err #" << WSAGetLastError() << "\n";
		closesocket(socket_);
		WSACleanup();
		return NULL;
	}
	return socket_;
}

void tcpSend(const char* str, SOCKET socket_)
{
	int res = send(socket_, str, strlen(str), 0);
	if (res > 0)
		std::cout << "Message sent" << "\n";
	else
		std::cerr << "Error while sending message" << "\n";
}