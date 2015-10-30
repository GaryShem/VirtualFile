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
	else //�������� ��������� ������ ������, ��� ����
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
	//���� ������ ���� ������, �� ��������� ������ �� ����� - ���� �� ���
	if (nSize > reservedSize)
	{
		//����������, ������� ����� ����������������� ������
		size_t newReserveSize = nSize*GrowFactor;
		if (newReserveSize > reservedSize) //�� ������ ������ - ����� ��� ���������� ���-������ ����������
			reservedSize = newReserveSize;
		else
			reservedSize = nSize;


		if (reservedSize % reserveMinimum != 0)
			reservedSize = reservedSize + reserveMinimum - (reservedSize%reserveMinimum);
		//����������� ������ � �������� ���� ��� ��������� ������
		void* newBuffer = VirtualAlloc(NULL, reservedSize, MEM_RESERVE, PAGE_READWRITE);
		if (newBuffer == NULL)
			throw EXCEPTION_ACCESS_VIOLATION;
		void* errorCheck = VirtualAlloc(newBuffer, commitedSize, MEM_COMMIT, PAGE_READWRITE);
		if (newBuffer == NULL)
			throw EXCEPTION_ACCESS_VIOLATION;
		if (buffer != NULL)
		{
			CopyMemory(newBuffer, buffer, fileSize);
			//����������� ������ ������
			VirtualFree(buffer, 0, MEM_RELEASE);
		}
		//��������������� ������ � ������ �������
		buffer = newBuffer;
	}
	//� ���� ��� �������, ��� ����������������� ������ ��� �������

	//���� ��� �� ������� ������������� ������
	//����� ��� ������� ������ �� �������� ���������
	if (nSize > commitedSize)
	{
		//�������� ����� �����, ���� �������, ��� ���� ������� ������� ��������
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