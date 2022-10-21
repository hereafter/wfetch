#pragma once

using namespace std;

class WFetchCharInfo
{
public:
	WFetchCharInfo();
	WFetchCharInfo(int x, int y, 
		uint16_t c,
		TCHAR* pv);

	~WFetchCharInfo();

public:
	int X() const;
	void X(int value);
	int Y() const;
	void Y(int value);
	TCHAR* Value() const;
	void Value(TCHAR* value);
	TCHAR Read();
	void Write(TCHAR v);

	bool IsColorChanging() const;
	uint16_t Color() const;
	void Color(uint16_t value);

private:
	int32_t _x;
	int32_t _y;
	uint16_t _c;
	TCHAR* _value;
};