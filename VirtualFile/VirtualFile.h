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

	void* GetBuffer();
private:
	float GrowFactor;
	size_t usedSize;
	size_t currentSize;
	size_t reservedSize;
	void* buffer;
	size_t offset;
};

/*
size_t offset;
void* buf;
void* res;
res = buf + offset; //ПЛОХО, система не даёт прибавлять к void*
res = (void*)((size_t)buf+offset); //ПРАВИЛЬНО
*/