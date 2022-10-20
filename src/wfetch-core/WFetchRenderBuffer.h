#pragma once

#include "WFetchCharInfo.h"

using namespace std;

class WFetchRenderBuffer
{
public:
	WFetchRenderBuffer();
	WFetchRenderBuffer(int cols, int rows);
	~WFetchRenderBuffer();

public:
	int CursorX() const;
	int CursorY() const;

public:
	void Clear();	
	void MoveTo(int x, int y);
	void MoveToNextCharacter();
	void MoveToLineBegin();
	void MoveToNextLine();
	void SetForegroundColor(int8_t c);
	void SetBackgroundColor(int8_t c);
	void ResetColors();
	void SetColors(vector<int>& fcs, vector<int>& bcs);

	TCHAR ReadChar();
	void WriteChar(TCHAR value);
	void WriteString(const TCHAR* value);
	void WriteBlockString(const TCHAR* value);
	
public:
	void RenderToDebug();
	void RenderToConsole();

private:
	int GetIndexFromCoords(int x, int y) const;
	WFetchCharInfo* const GetCurrentCharInfo() const;
	bool IsCoordsWithinRange(int x, int y) const;
	int GetValuesSize();

private:
	bool IsRowEmpty(int r) const;
	void ProcessColors(wstring const& controls);


private:
	int _cursorX;
	int _cursorY;
	int _rows;
	int _columns;
	shared_ptr<WFetchCharInfo[]> _infos;
	shared_ptr<TCHAR[]> _values;

	map<wstring, int> _foregroundColors;
	map<wstring, int> _backgroundColors;
	int _defaultColor;
};

