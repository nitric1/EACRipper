#pragma once

#define WINVER 0x0700
#define _WIN32_WINDOWS 0x0410
#define _WIN32_WINNT 0x0700
#define _WIN32_IE 0x0700

namespace EACRipper {}
namespace ER = EACRipper;

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

// Standard C/C++ Library Inclusion

#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cinttypes>

#include <algorithm>
#include <exception>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <list>
#include <map>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <queue>

using std::max; using std::min;

// Boost Library Inclusion

#include <boost/cast.hpp>
#include <boost/lexical_cast.hpp>

// ICU Library Inclusion

#include <unicode/ucnv.h>
#include <unicode/ucsdet.h>

// Windows API Inclusion

#include <windows.h>
#include <windowsx.h>

#include <commctrl.h>
#include <commdlg.h>
#include <dlgs.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>

#include <gdiplus.h>

// Other Common Inclusion

#include "ERTypes.h"

#include "resource.h"
