#include "stdafx.h"
#include "VirtualFile.h"
#include "windows.h"
#include <algorithm>

using namespace std;

VirtualFile::VirtualFile(float fGrowFactor)
{
	GrowFactor = fGrowFactor;
	commitedSize= 0;
	fileSize = 0;
	reservedSize = 0;
	offset = 0;
	buffer = NULL;
	SYSTEM_INFO si;
	GetSystemInfo(&si);
	commitMinimum = si.dwPageSize;
	reserveMinimum = si.dwAllocationGranularity;
}

VirtualFile::~VirtualFile()
{
	if (buffer != NULL)
	{
		VirtualFree(buffer, 0, MEM_RELEASE);
	}
}

size_t VirtualFile::Read(void* pDest, size_t nSize)
{
	if (offset + nSize <= fileSize)
	{
		CopyMemory(pDest, (void*)((size_t)buffer + offset), nSize);
		offset += nSize;
		return nSize;
	}
	else //пытаемся прочитать данных больше, чем есть
	{
		size_t length = fileSize - offset;
		CopyMemory(pDest, (void*)((size_t)buffer + offset), length);
		return length;
	}
}

void VirtualFile::Write(void* pSrc, size_t nSize)
{
	size_t newFileSize = max(fileSize, offset + nSize);
	void* check = SetSize(newFileSize);
	if (check == NULL)
		throw EXCEPTION_ACCESS_VIOLATION;
	fileSize = newFileSize;
	CopyMemory((void*)((size_t)buffer+offset), pSrc, nSize);
}

size_t VirtualFile::Seek(SeekPosition pos, int shift)
{
	size_t startingPosition;
	if (pos == SeekPosition::begin)
		startingPosition = 0;
	else if (pos == SeekPosition::current)
		startingPosition = offset;
	else //if (pos == SeekPosition::end)
		startingPosition = fileSize;

	if (shift > 0)
	{
		offset = startingPosition + shift;
		if (offset > fileSize)
			offset = fileSize;
	}
	if (shift < 0)
	{
		if ((size_t)abs(shift) > startingPosition)
			offset = 0;
		else
			offset = startingPosition - (size_t)abs(shift);
	}

	return offset;
}

size_t VirtualFile::GetSize()
{
	return fileSize;
}

void* VirtualFile::SetSize(size_t nSize)
{
	//если размер стал меньше, то оставляем резерв на месте - мало ли что
	if (nSize > reservedSize)
	{
		//определяем, сколько будет зарезервированной памяти
		size_t newReserveSize = nSize*GrowFactor;
		if (newReserveSize > reservedSize) //на всякий случай - вдруг при округлении что-нибудь потеряется
			reservedSize = newReserveSize;
		else
			reservedSize = nSize;


		if (reservedSize % reserveMinimum != 0)
			reservedSize = reservedSize + reserveMinimum - (reservedSize%reserveMinimum);
		//резервируем память и копируем туда уже имеющиеся данные
		void* newBuffer = VirtualAlloc(NULL, reservedSize, MEM_RESERVE, PAGE_READWRITE);
		if (newBuffer == NULL)
			throw EXCEPTION_ACCESS_VIOLATION;
		void* errorCheck = VirtualAlloc(newBuffer, commitedSize, MEM_COMMIT, PAGE_READWRITE);
		if (newBuffer == NULL)
			throw EXCEPTION_ACCESS_VIOLATION;
		if (buffer != NULL)
		{
			CopyMemory(newBuffer, buffer, fileSize);
			//освобождаем старый регион
			VirtualFree(buffer, 0, MEM_RELEASE);
		}
		//переприсваиваем буффер к новому региону
		buffer = newBuffer;
	}
	//с этих пор считаем, что зарезервированной памяти нам хватает

	//если нам не хватает закоммиченной памяти
	//здесь уже бережно следим за очисткой страничек
	if (nSize > commitedSize)
	{
		//коммитим такой кусок, чтоб хватало, при этом кратный размеру страницы
		commitedSize = nSize;
		if (commitedSize % commitMinimum != 0)
			commitedSize = commitedSize + commitMinimum - (commitedSize % commitMinimum);
		VirtualAlloc(buffer, commitedSize, MEM_COMMIT, PAGE_READWRITE);
	}
	else //if (nSize < commitedSize)
	{
		size_t decommitBegin = commitedSize;
		while (decommitBegin - commitMinimum > nSize)
			decommitBegin -= commitMinimum;
		if (decommitBegin != commitedSize)
		{
			VirtualFree((void*)((size_t)buffer + decommitBegin + 1), commitedSize - decommitBegin - 1, MEM_DECOMMIT);
			fileSize = min(fileSize, nSize);
		}
	}

	return buffer;
}

void* VirtualFile::GetBuffer()
{
	return buffer;
}