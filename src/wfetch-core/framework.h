#pragma once

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#define _WIN32_DCOM

#include <string>
#include <sstream>

#include <tchar.h>
#include <Windows.h>
#include <WbemIdl.h>

#include <winrt/base.h>

#include <winrt/windows.foundation.h>
#include <winrt/windows.foundation.collections.h>
#include <winrt/windows.system.h>