#ifndef SCC_H
#define SCC_H

#pragma warning(disable:4133)
#pragma GCC diagnostic ignored "-Wincompatible-pointer-types"
#pragma GCC diagnostic ignored "-Wlogical-op-parentheses"
#pragma GCC diagnostic ignored "-Wint-conversion"
#pragma GCC diagnostic ignored "-Wint-to-void-pointer-cast"

#define _CRT_SECURE_NO_WARNINGS

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
