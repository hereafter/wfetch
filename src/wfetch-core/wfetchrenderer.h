#pragma once

#include "wfechcharinfo.h"
using namespace std;

class WFetchRenderer
{
public:
	WFetchRenderer();
	WFetchRenderer(int cols, int rows);
	~WFetchRenderer();

public:
	shared_ptr<WFetchCharInfo[]> Infos;
	shared_ptr<TCHAR[]> Values;
};

