#include "pch.h"
#include "WFetchCharInfo.h"

WFetchCharInfo::WFetchCharInfo() :
	_x(-1), _y(-1),_c(-1),
	_value(nullptr)
{}

WFetchCharInfo::WFetchCharInfo(int x, int y,
	uint16_t c,
	TCHAR* pv):
	_x(x), _y(y), _c(c),
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
	return _c != 0xff;
}

uint16_t WFetchCharInfo::Color() const
{
	return _c;
}

void WFetchCharInfo::Color(uint16_t value)
{
	_c = value;
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