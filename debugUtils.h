#pragma once
#include <windows.h>
#include <typeinfo>
#include <string>


inline void DebugLog(const char* message)
{
	OutputDebugStringA(message);
}

