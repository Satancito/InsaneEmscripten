#pragma once
#ifndef INSANE_DEFAULT_H
#define INSANE_DEFAULT_H
#define STDCALL _stdcall
#define CDECL _cdecl

#if defined (_WIN32) 
#define WINDOWSLIB 1
#define DLLCALL STDCALL
#define DLLIMPORT _declspec(dllimport)
#define DLLEXPORT _declspec(dllexport)
#define DLLPRIVATE

#define NOMINMAX
#include <Windows.h>
#include <objbase.h>
#include <filesystem>
#include <intrin.h>
#include <conio.h>

#elif defined (__ANDROID__) || defined(ANDROID)//Android
#define ANDROIDLIB 1
#define DLLCALL CDECL
#define DLLIMPORT
#define DLLEXPORT __attribute__ ((visibility ("default")))
#define DLLPRIVATE __attribute__ ((visibility ("hidden")))


#elif defined (__APPLE__)//iOS, Mac OS
#define MACOSLIB 1
#define DLLCALL CDECL
#define DLLIMPORT
#define DLLEXPORT __attribute__ ((visibility ("default")))
#define DLLPRIVATE __attribute__ ((visibility ("hidden")))

#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__) || defined(__linux) || defined(linux)//_Ubuntu - Fedora - Centos - RedHat
#define LINUXLIB 1
#include <cpuid.h>
#include <experimental/filesystem>
#include <unistd.h>
#include <termios.h>
#define DLLCALL CDECL
#define DLLIMPORT
#define DLLEXPORT __attribute__ ((visibility ("default")))
#define DLLPRIVATE __attribute__ ((visibility ("hidden")))
#define CoTaskMemAlloc(p) malloc(p)
#define CoTaskMemFree(p) free(p)

#elif defined (__EMSCRIPTEN__)
#define EMSCRIPTENLIB 1
#include <unistd.h>
#include <termios.h>
#define DLLCALL 
#define DLLIMPORT
#define DLLEXPORT __attribute__ ((visibility ("default")))
#define DLLPRIVATE __attribute__ ((visibility ("hidden")))

#endif

#define EXTERNC extern "C"

#if defined (INSANE_EXPORTS)
#define DLLSPEC DLLEXPORT
#else
#define DLLSPEC DLLIMPORT
#endif

//INCLUDES
#include  <iomanip>
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

#if WINDOWSLIB /* Windows */
#if _WIN64
#define IS64 1
#else
#define IS32 1
#endif
typedef long long Int64;
typedef unsigned long long UInt64;
typedef std::wstring DefaultString;

#elif LINUXLIB /* Linux */
#if __x86_64__ || __ppc64__
#define IS64 1
typedef long Int64;
typedef unsigned long UInt64;
typedef std::string DefaultString;
#else
#define IS32 1
typedef long long Int64;
typedef unsigned long long UInt64;
#endif

#elif EMSCRIPTENLIB /* Emscripten*/
typedef long long Int64;
typedef unsigned long long UInt64;
typedef std::string DefaultString;


#elif defined MACOSLIB /* MacOS*/
#endif

typedef unsigned int UInt32;
typedef int Int32;
typedef unsigned char UChar;
typedef char Char;
typedef wchar_t WChar;
typedef time_t TimeT;
typedef size_t SizeT;
typedef std::string String;
typedef std::wstring WString;


using namespace std::string_literals;

#define PUBLIC_MEMBERS public:
#define PRIVATE_MEMBERS private:

#define ZERO 0
#define ONE 1

#endif // !INSANE_DEFAULT_H



/*

Temas de interes.
� Standard data types on UNIX, Linux, and Windows.
Sobre los tipos de datos est�ndares de C++. Y las diferencias de tama�o en bytes de cada tipo dependiendo de
la plataforma y la arquitectura.
https://www.ibm.com/support/knowledgecenter/en/SSFKSJ_9.0.0/com.ibm.mq.ref.dev.doc/q104610_.html

� Tipos de datos Visual C++.
https://docs.microsoft.com/en-us/cpp/cpp/data-type-ranges?view=vs-2017

� Tipos de datos C++.
https://es.cppreference.com/w/cpp/language/types

*/
