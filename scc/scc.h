#ifndef SCC_H
#define SCC_H

#if _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#pragma warning(disable:4133)
#else
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wlogical-op-parentheses"
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wint-to-void-pointer-cast"
#endif

#include <stddef.h>
#include "sccobj.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <setjmp.h>
#include "err.h"

#if _WIN32
#include <malloc.h>
#elif __APPLE__
#include <mm_malloc.h>
#endif

#define DEBUG _DEBUG

#endif // !SCC_H
