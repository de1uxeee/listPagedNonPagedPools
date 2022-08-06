#pragma once
#include <ntifs.h>

#define DebugLog(...) DbgPrintEx(0, 0, __VA_ARGS__)

typedef struct _POOL_TRACKER_TABLE
{
    LONG Key;				// 0x0
    ULONGLONG NonPagedBytes;				// 0x8
    ULONGLONG NonPagedAllocs;				// 0x10
    ULONGLONG NonPagedFrees;				// 0x18
    ULONGLONG PagedBytes;				// 0x20
    ULONGLONG PagedAllocs;				// 0x28
    ULONGLONG PagedFrees;				// 0x30
    LONGLONG AvailableLimit[2];				// 0x38
} POOL_TRACKER_TABLE, * PPOOL_TRACKER_TABLE;

typedef struct _POOL_TRACKER_BIG_PAGES
{
    ULONGLONG Va;				// 0x0
    ULONG Key;				// 0x8
    ULONG Flags;
    ULONGLONG NumberOfBytes;				// 0x10
} POOL_TRACKER_BIG_PAGES, * PPOOL_TRACKER_BIG_PAGES;