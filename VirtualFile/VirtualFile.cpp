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
		
	}
	else
	{
		
	}
}

void VirtualFile::Write(void* pSrc, size_t nSize)
{
}

size_t VirtualFile::Seek(SeekPosition pos, int offset)
{
}

size_t VirtualFile::GetSize()
{
}

void* VirtualFile::GetBuffer()
{
	
}