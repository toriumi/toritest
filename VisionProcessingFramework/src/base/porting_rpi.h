/**
* @file      porting_rpi.h
* @brief     Header for porting rpi
* @author    Vision Processing Community
* @copyright 2016 Vision Processing Community. All rights reserved.
*/
#pragma once
/*#include <stdio.h>*/
/*#include <wx/wx.h>*/

#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif

typedef char INT8;
typedef unsigned char UINT8;
typedef short INT16;            // NOLINT
typedef unsigned short UINT16;  // NOLINT
typedef int INT32;
typedef unsigned int UINT32;
typedef unsigned int HANDLE;
typedef int BOOL;
typedef UINT8 BOOLEAN;
typedef UINT8 BYTE;
typedef UINT16 WORD;

typedef unsigned long DWORD;  // NOLINT
typedef char TCHAR;

typedef char *PSTR, *LPSTR, *LPCSTR, *LPCTSTR;

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif

/*#define __DEBUG */
#ifdef __DEBUG
#define DEBUG_PRINT(...) (fprintf(stderr, __VA_ARGS__))
#else
#define DEBUG_PRINT(...)
#endif
