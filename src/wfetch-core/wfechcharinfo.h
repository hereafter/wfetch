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
	TCHAR* Value() const;
	void Value(TCHAR* value);
	TCHAR Read();
	void Write(TCHAR v);

	bool IsColorChanging() const;
	int8_t ForegroundColor() const;
	void ForegroundColor(int8_t value);
	int8_t BackgroundColor() const;
	void BackgroundColor(int8_t value);

private:
	int32_t _x;
	int32_t _y;
	int8_t _fc;
	int8_t _bc;
	TCHAR* _value;
};