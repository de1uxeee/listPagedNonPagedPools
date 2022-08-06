#include "../detection/detection.h"

void Unload(PDRIVER_OBJECT obj)
{
	UNREFERENCED_PARAMETER(obj);
}

NTSTATUS DriverEntry(PDRIVER_OBJECT obj, PUNICODE_STRING path)
{
	UNREFERENCED_PARAMETER(path);

	obj->DriverUnload = Unload;

	outputPoolTrackTable();
	outputBigPageTable();
	outputNonPagedPools();
	outputPagedPools();

	return STATUS_SUCCESS;
}