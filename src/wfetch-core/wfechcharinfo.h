#pragma once

using namespace std;

class WFetchCharInfo
{
public:
	WFetchCharInfo();
	WFetchCharInfo(int x, int y, 
		int8_t fc, int8_t bc, 
		TCHAR* pv);

	~WFetchCharInfo();

public:
	int X() const;
	void X(int value);
	int Y() const;
	void Y(int value);

	TCHAR Value() const;
	void Value(TCHAR value);

private:
	int32_t _x;
	int32_t _y;
	int8_t _fc;
	int8_t _bc;
	TCHAR* _value;
};