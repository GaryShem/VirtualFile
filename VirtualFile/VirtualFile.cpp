#include "stdafx.h"
#include "VirtualFile.h"
#include "windows.h"

using namespace std;

VirtualFile::VirtualFile(float fGrowFactor)
{
	GrowFactor = fGrowFactor;
	currentSize = 0;
	usedSize = 0;
	reservedSize = 0;
	offset = 0;
	buffer = NULL;
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
	if (offset + nSize <= currentSize)
	{
		CopyMemory(pDest, (void*)((size_t)buffer + offset), nSize);
		offset += nSize;
		return nSize;
	}
	else //пытаемся прочитать данных больше, чем есть
	{
		size_t length = currentSize - offset;
		CopyMemory(pDest, (void*)((size_t)buffer + offset), length);
		return length;
	}
}

void VirtualFile::Write(void* pSrc, size_t nSize)
{

}

size_t VirtualFile::Seek(SeekPosition pos, int offset)
{
	return 0;
}

size_t VirtualFile::GetSize()
{
	return usedSize;
}

void* VirtualFile::SetSize(size_t nSize)
{
	return NULL;
}

void* VirtualFile::GetBuffer()
{
	return buffer;
}