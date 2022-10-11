#include "pch.h"
#include "wfechcharinfo.h"

WFetchCharInfo::WFetchCharInfo() :
	_x(-1), _y(-1),
	_fc(-1), _bc(-1),
	_value(nullptr)
{}

WFetchCharInfo::WFetchCharInfo(int x, int y,
	int8_t fc, int8_t bc, 
	TCHAR* pv):
	_x(x), _y(y), _fc(fc), _bc(bc),
	_value(pv)
{}

WFetchCharInfo::~WFetchCharInfo()
{
	_value = nullptr;
}

int WFetchCharInfo::X() const
{
	return _x;
}

void WFetchCharInfo::X(int value)
{
	_x = value;
}

int WFetchCharInfo::Y() const
{
	return _y;
}

void WFetchCharInfo::Y(int value)
{
	_y = value;
}

TCHAR WFetchCharInfo::Value() const
{
	if (_value == nullptr) return 0;
	return *_value;
}

void WFetchCharInfo::Value(TCHAR value)
{
	if (_value == nullptr) throw winrt::hresult{E_POINTER};
	*_value = value;
}