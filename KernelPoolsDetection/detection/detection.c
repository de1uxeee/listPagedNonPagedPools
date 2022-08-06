#include "detection.h"

#define PTBP_GET_POOL_TYPE(Flags) ((Flags & 0xFFF00) >> 8)

void outputPoolTrackTable()
{
	DebugLog("Pool track table:\n");

	PVOID tableSizePtr = (PVOID)0xffff84841846f000;
	PVOID tableAddr = (PVOID)0xffff84841846f018;

	PPOOL_TRACKER_TABLE poolTable = *(PPOOL_TRACKER_TABLE*)tableAddr;
	SIZE_T tableSize = *(SIZE_T*)tableSizePtr;

	if (poolTable && tableSize)
	{
		for (SIZE_T i = 0; i < tableSize; i++)
		{
			PPOOL_TRACKER_TABLE currentTable = &poolTable[i];
			char tag[5] = { 0 };

			if (currentTable->Key > 0)
			{
				memcpy(tag, &currentTable->Key, 0x4);
				if (tag[3] > '\0')
					DebugLog("tag: %s, paged bytes: %llx, non-paged bytes: %llx\n", tag, currentTable->PagedBytes, currentTable->NonPagedBytes);
			}
		}
	}
	
}

void outputBigPageTable()
{
	DebugLog("Big page table:\n");

	PVOID tableSizePtr = (PVOID)0xffff84841845b110;
	PVOID tableAddr = (PVOID)0xffff84841845b210;

	PPOOL_TRACKER_BIG_PAGES poolTable = *(PPOOL_TRACKER_BIG_PAGES*)tableAddr;
	SIZE_T tableSize = *(SIZE_T*)tableSizePtr;

	if (poolTable && tableSize)
	{
		for (SIZE_T i = 0; i < tableSize; i++)
		{
			PPOOL_TRACKER_BIG_PAGES currTable = &poolTable[i];
			char tag[5] = { 0 };

			if (currTable->Key > 0)
			{
				memcpy(tag, &currTable->Key, 0x4);
				if (tag[3] > '\0' && currTable->Va > 0xffff000000000000)
					DebugLog("tag %s, pool type: %hu, size: 0x%llx, virtual addr: 0x%llx\n", tag, PTBP_GET_POOL_TYPE(currTable->Flags), currTable->NumberOfBytes, currTable->Va);
			}
		}
	}
}

void outputNonPagedPools()
{
	DebugLog("NonPaged Pools list:\n");

	PVOID listAddr = (PVOID)0xffff84841845c930;

	if (listAddr)
	{
		PGENERAL_LOOKASIDE nonPagedList = *(PGENERAL_LOOKASIDE*)listAddr;
		PGENERAL_LOOKASIDE currList = (PGENERAL_LOOKASIDE)((UINT_PTR)nonPagedList - 0x40);
		
		while (TRUE)
		{
			char tag[5] = { 0 };
			memcpy(tag, &currList->Tag, 0x4);

			if (tag[3] > '\0')
				DebugLog("tag %s, size: 0x%lx, total allocs: 0x%lx\n", tag, currList->Size, currList->TotalAllocates);

			currList = (PGENERAL_LOOKASIDE)((UINT_PTR)currList->ListEntry.Flink - 0x40);

			if (currList == (PGENERAL_LOOKASIDE)((UINT_PTR)nonPagedList - 0x40))
				break;
		}
	}
}

void outputPagedPools()
{
	DebugLog("Paged Pools list:\n");

	PVOID listAddr = (PVOID)0xffff84841845c828;

	if (listAddr)
	{
		PGENERAL_LOOKASIDE pagedList = *(PGENERAL_LOOKASIDE*)listAddr;
		PGENERAL_LOOKASIDE currList = (PGENERAL_LOOKASIDE)((UINT_PTR)pagedList - 0x40);

		while (TRUE)
		{
			char tag[5] = { 0 };
			memcpy(tag, &currList->Tag, 0x4);

			if (tag[3] > '\0')
				DebugLog("tag %s, size: 0x%lx, total allocs: 0x%lx\n", tag, currList->Size, currList->TotalAllocates);

			currList = (PGENERAL_LOOKASIDE)((UINT_PTR)currList->ListEntry.Flink - 0x40);

			if (currList == (PGENERAL_LOOKASIDE)((UINT_PTR)pagedList - 0x40))
				break;
		}
	}
}