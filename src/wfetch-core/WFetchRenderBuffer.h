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
	void SetColor(uint16_t c);
	void ResetColors();
	void SetColors(vector<uint16_t>& colors);

	TCHAR ReadChar();
	void WriteChar(TCHAR value);
	void WriteString(const TCHAR* value);
	void WriteBlockString(const TCHAR* value);
	void WriteColorPalette(int x, int y);
	void WriteColorPaletteCell(uint16_t color);
	
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

	map<wstring, uint16_t> _colors;
	uint16_t _defaultColor;
};

