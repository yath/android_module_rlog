/*****************************************************************************
 * Author:   Vadim Zeitlin <vadim@wxwidgets.org>
 *
 *****************************************************************************
 * Copyright (c) 2004 Vadim Zeitlin
 *
 * This library is free software; you can distribute it and/or modify it under
 * the terms of the GNU Lesser General Public License (LGPL), as published by
 * the Free Software Foundation; either version 2.1 of the License, or (at your
 * option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the LGPL in the file COPYING for more
 * details.
 *
 */

#include "config.h"
#include "RLogTime.h"

#ifdef _WIN32

#include <windows.h>

#define HAVE_QUERYPERFORMANCECOUNTER 1

typedef __int64 rlog_time_interval;

#if HAVE_QUERYPERFORMANCECOUNTER

typedef LARGE_INTEGER rlog_time;

const char *rlog_time_unit()
{
    return "clock cycles";
}

void rlog_get_time(rlog_time *pt)
{
    QueryPerformanceCounter(pt);
}

rlog_time_interval rlog_time_diff(const rlog_time& end, const rlog_time& start)
{
    long long llEnd, llStart;
    memcpy(&llEnd, &end, sizeof(long long));
    memcpy(&llStart, &start, sizeof(long long));
    return llEnd - llStart;
}

#else // !HAVE_QUERYPERFORMANCECOUNTER

typedef FILETIME rlog_time;

const char *rlog_time_unit()
{
    return "usec";
}

void rlog_get_time(rlog_time *pt)
{
    GetSystemTimeAsFileTime(pt);
}

rlog_time_interval rlog_time_diff(const rlog_time& end, const rlog_time& start)
{
    ULONGLONG ullEnd, ullStart;
    memcpy(&ullEnd, &end, sizeof(ULONGLONG));
    memcpy(&ullStart, &start, sizeof(ULONGLONG));
    return 10*(ullEnd - ullStart);
}

#endif // HAVE_QUERYPERFORMANCECOUNTER

void sleep(int seconds)
{
    ::Sleep(seconds * 1000);
}

#else // Unix

#include <sys/time.h>

#ifdef HAVE_ASM_MSR_H

typedef uint32_t u32; 
typedef uint64_t u64; 
#include <asm/msr.h> 

const char * rlog_time_unit()
{
    return "clock cycles";
}

void rlog_get_time(uint64_t *pt)
{
    rdtscll( *pt );
}

rlog_time_interval rlog_time_diff(const rlog_time &end, const rlog_time &start )
{
    return end - start;
}

#else // !HAVE_TSC

#include <unistd.h>

typedef timeval rlog_time;
typedef long rlog_time_interval;

const char * rlog_time_unit()
{
    return "usec";
}

void rlog_get_time(rlog_time *pt)
{
    gettimeofday( pt, 0 );
}

rlog_time_interval rlog_time_diff( const rlog_time &end, const rlog_time &start )
{
    return (end.tv_sec - start.tv_sec) * 1000 * 1000 + 
	(end.tv_usec - start.tv_usec);
}

#endif // HAVE_TSC/!HAVE_TSC

void rlog_sleep(int seconds)
{
    ::sleep(seconds);
}

#endif // Win32/Unix

