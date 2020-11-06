#include <Windows.h>
#include "utils.h"

const wchar_t* GetWC(const char* c)
{
    const size_t cSize = strlen(c) + 1;
    wchar_t* wc = new wchar_t[cSize];
    size_t outSize;
    mbstowcs_s(&outSize, wc, cSize, c, cSize - 1);
    return wc;
}