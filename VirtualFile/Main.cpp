// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VirtualFile.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	VirtualFile vf;
	int a = 4;
	vf.Write(&a, sizeof(a));
	a = 5;
	vf.Seek(VirtualFile::SeekPosition::begin, 0);
	vf.Read(&a, sizeof(a));
	std::cout << a << std::endl;
	std::cout << "cycle" << std::endl;
	vf.Seek(VirtualFile::SeekPosition::begin, 0);
	for (int i = 10; i > 0; i--)
		vf.Write(&i, sizeof(i));
	vf.Seek(VirtualFile::SeekPosition::begin, 0);
	for (int i = 0; i < 10; i++)
	{
		int b;
		vf.Read(&b, sizeof(b));
		std::cout << i<< " : " << b << std::endl;
	}
}
