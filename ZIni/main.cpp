// ZIni.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "ZIni.h"

int _tmain(int argc, _TCHAR* argv[])
{
	ZIni ini("e:\\abc.ini");

	double dnumber = ini.getDouble("xxxx", "yyyy", 1.5);
	printf("%lf", dnumber);
	//printf("%d\n", ss.length());
	//printf("%s\n", ss.c_str());

	system("pause");
	return 0;
}

