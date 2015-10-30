// Main.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "VirtualFile.h"
#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	int a = 4;
	VirtualFile vf;
	vf.Write(&a, sizeof(a));
	a = 5;
	vf.Seek(VirtualFile::begin, 0);
	vf.Read(&a, sizeof(a));
	std::cout << a;
}

