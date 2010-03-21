#pragma once

#define WINVER 0x0600
#define _WIN32_WINDOWS 0x0410
#define _WIN32_WINNT 0x0600
#define _WIN32_IE 0x0600

#define EACRIPPER_VERSION L"0.4.0ет"

namespace EACRipper {}

namespace ER = EACRipper;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <algorithm>
#include <limits>
#include <exception>
#include <stdexcept>

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
