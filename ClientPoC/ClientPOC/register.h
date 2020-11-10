#ifndef REGISTER_H
#define REGISTER_H

#include "utils.h"
#include <Windows.h>
#include <winreg.h>

LONG SetRegValue(const wchar_t* path);

#endif // !REGISTER_H
