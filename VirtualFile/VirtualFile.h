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
	float GrowFactor; // ��� ���?
	size_t fileSize; //����� ������, ��� ������� �������
	size_t commitedSize; //����� commited ������
	size_t reservedSize; //����� ����������������� ������
	void* buffer; //������ �����
	size_t offset; //������� ��������� �������
	size_t commitMinimum;
	size_t reserveMinimum;
};

/*
size_t offset;
void* buf;
void* res;
res = buf + offset; //�����, ������� �� ��� ���������� � void*
res = (void*)((size_t)buf+offset); //���������
*/