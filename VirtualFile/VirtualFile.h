#pragma once
class VirtualFile
{
public:
	enum SeekPosition
	{
		begin = 0,
		current,
		end
	};
	float m_fGrowFactor;

	VirtualFile(float fGrowFactor = 1.5);
	~VirtualFile(void);

	size_t Read(void* pDest, size_t nSize);
	void Write(void *pSrc, size_t nSize);
	size_t Seek(SeekPosition pos, int offset);
	size_t GetSize();
	void* SetSize(size_t nSize);

	void* GetBuffer();
private:
	float GrowFactor; // это что?
	size_t fileSize; //объём памяти, уже занятый данными
	size_t commitedSize; //объём commited памяти
	size_t reservedSize; //объём зарезервированной памяти
	void* buffer; //начало файла
	size_t offset; //текущее положение курсора
	size_t commitMinimum;
	size_t reserveMinimum;
};

/*
size_t offset;
void* buf;
void* res;
res = buf + offset; //ПЛОХО, система не даёт прибавлять к void*
res = (void*)((size_t)buf+offset); //ПРАВИЛЬНО
*/