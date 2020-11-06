#include "register.h"

LONG SetRegValue(const wchar_t* path)
{
    HKEY hKey;
    LONG status;

    if ((status = RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("SYSTEM\\CurrentControlSet\\Services\\Netlogon\\Parameters"), 0, KEY_ALL_ACCESS, &hKey)) == ERROR_SUCCESS && (hKey != NULL))
    {
        DWORD dwType;
        DWORD dwLen = 1000;
        char buffer[1000];
        const char* name = "FullSecureChannelProtection";
        if ((status = RegQueryValueExA(hKey, name, NULL, &dwType, (LPBYTE)buffer, &dwLen)) == ERROR_SUCCESS && dwType == REG_DWORD && buffer)
        {
            sprintf_s(buffer, "%u", *(DWORD*)buffer);
            std::cout << "Value of FullSecureChannelProtection before : " << buffer << "\n";
            int value = 1;
            if ((status = RegSetValueExA(hKey, name, 0, REG_DWORD, (BYTE*)&value, sizeof(value))) == ERROR_SUCCESS)
                std::cout << "Value changed" << "\n";
            else
                std::cout << "Error while changing value" << "\n";
        }
        RegCloseKey(hKey);
    }
    return status;
}