#pragma once

#define WINVER 0x0700
#define _WIN32_WINDOWS 0x0410
#define _WIN32_WINNT 0x0700
#define _WIN32_IE 0x0700

#define EACRIPPER_TITLE L"EACRipper"
#define EACRIPPER_VERSION L"0.4.0\x03B2"

namespace EACRipper {}

namespace ER = EACRipper;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <cstdio>
#include <cstdlib>

#include <cinttypes>

#include <exception>
#include <stdexcept>

#include <algorithm>
#include <functional>
#include <memory>

#include <limits>

#include <list>
#include <map>
#include <vector>
#include <string>

#include <iostream>
#include <sstream>
#include <iomanip>

using std::max; using std::min;

#include <windows.h>
#include <windowsx.h>

#include <tchar.h>

#include <commdlg.h>
#include <commctrl.h>
#include <dlgs.h>

#include <shlwapi.h>
#include <shlobj.h>

#include <shellapi.h>

#include <gdiplus.h>

#include "resource.h"
