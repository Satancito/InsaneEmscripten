#pragma once
#ifndef INSANE_BASE_H
#define INSANE_BASE_H

#define STDCALL _stdcall
#define CDECL _cdecl
#define EXTERNC extern "C"

#include <iomanip>
#include <sstream>
#include <iostream>
#include <string>
#include <ctime>
#include <climits>
#include <array>
#include <random>
#include <functional>
#include <vector>
#include <locale>
#include <cstring>
#include <algorithm>
#include <chrono>
#include <vector>
#include <set>
#include <cstdlib>

// WINDOWS
#if (_WIN32)
#include <Windows.h>
#include <conio.h>
#define WINDOW_PLATFORM 1
#define DLLCALL STDCALL
#define DLLIMPORT _declspec(dllimport)
#define DLLEXPORT _declspec(dllexport)
#define DLLPRIVATE
#define NOMINMAX
typedef std::wstring DefaultString;
#if _WIN64
#define IS64 1
#else
#define IS32 1
#endif

//EMSCRIPTEN
#elif defined(__EMSCRIPTEN__)
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <unistd.h>
#include <termios.h>
#define EMSCRIPTEN_PLATFORM 1
#define DLLCALL
#define DLLIMPORT
#define DLLEXPORT __attribute__((visibility("default")))
#define DLLPRIVATE __attribute__((visibility("hidden")))
typedef std::string DefaultString;

// LINUX - Ubuntu, Fedora, , Centos, Debian, RedHat
#elif (__LINUX__ || __gnu_linux__ || __linux__ || __linux || linux)
#define LINUX_PLATFORM 1
#include <unistd.h>
#include <termios.h>
#define DLLCALL CDECL
#define DLLIMPORT
#define DLLEXPORT __attribute__((visibility("default")))
#define DLLPRIVATE __attribute__((visibility("hidden")))
#define CoTaskMemAlloc(p) malloc(p)
#define CoTaskMemFree(p) free(p)
typedef std::string DefaultString;
#if __x86_64__ || __ppc64__
#define IS64 1
#else
#define IS32 1
#endif

//ANDROID
#elif (__ANDROID__ || ANDROID)
#define ANDROID_PLATFORM 1
#define DLLCALL
#define DLLIMPORT
#define DLLEXPORT __attribute__((visibility("default")))
#define DLLPRIVATE __attribute__((visibility("hidden")))
typedef std::string DefaultString;

//MACOS
#elif defined(__APPLE__)
#include <unistd.h>
#include <termios.h>
#define DLLCALL
#define DLLIMPORT
#define DLLEXPORT __attribute__((visibility("default")))
#define DLLPRIVATE __attribute__((visibility("hidden")))
typedef std::string DefaultString;
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE && TARGET_IPHONE_SIMULATOR
#define IOS_SIMULATOR_PLATFORM 1
#elif TARGET_OS_IPHONE
#define IOS_PLATFORM 1
#elif TARGET_OS_MAC
#define MACOS_PLATFORM 1
#else

#endif

#endif

#if defined(INSANE_EXPORTS)
#define DLLSPEC DLLEXPORT
#else
#define DLLSPEC DLLIMPORT
#endif

typedef __UINT32_TYPE__ UInt32;
typedef __INT32_TYPE__ Int32;
typedef __UINT64_TYPE__ UInt64;
typedef __INT64_TYPE__ Int64;

typedef unsigned char UChar;
typedef char Char;
typedef wchar_t WChar;
typedef time_t Time;
typedef size_t Size;
typedef std::string String;
typedef std::wstring WString;

using namespace std::string_literals;

#endif // !INSANE_BASE_H


