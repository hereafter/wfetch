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

TCHAR* WFetchCharInfo::Value() const
{
	return _value;
}

void WFetchCharInfo::Value(TCHAR* value)
{
	_value = value;
}

bool WFetchCharInfo::IsColorChanging() const
{
	return _fc >= 0 || _bc >= 0;
}

int8_t WFetchCharInfo::ForegroundColor() const
{
	return _fc;
}

void WFetchCharInfo::ForegroundColor(int8_t value)
{
	_fc = value;
}

int8_t WFetchCharInfo::BackgroundColor() const
{
	return _bc;
}

void WFetchCharInfo::BackgroundColor(int8_t value)
{
	_bc = value;
}

TCHAR WFetchCharInfo::Read()
{
	if (_value == nullptr) return 0;
	return *_value;
}
void WFetchCharInfo::Write(TCHAR v)
{
	if (_value == nullptr) return;
	*_value = v;
}