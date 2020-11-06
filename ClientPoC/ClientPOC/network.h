#ifndef NETWORK_H
#define NETWORK_H

#include "utils.h"
#include <winsock2.h>
#include <Windows.h>
#include <ws2tcpip.h>

SOCKET tcpConnect();
void tcpSend(const char* str, SOCKET socket_);
#endif // !NETWORK_H
