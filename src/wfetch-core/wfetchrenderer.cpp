#include "pch.h"
#include "wfetchrenderer.h"
#include <iostream>
#include <sstream>

WFetchRenderer::WFetchRenderer():
	WFetchRenderer(255, 100)
{}

WFetchRenderer::WFetchRenderer(int cols, int rows):
	_rows(rows),
	_columns(cols),
	_cursorX(0),
	_cursorY(0)
{
	auto count = rows * cols;
	auto values = make_shared<TCHAR[]>(count);
	auto infos = make_shared<WFetchCharInfo[]>(count);

	auto offset = 0;
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			auto&& info = infos[offset];
			info.X(r); info.Y(c);
			info.Value(&values[offset]);
			offset++;
		}
	}

	_infos = infos;
	_values = values;
}

WFetchRenderer::~WFetchRenderer()
{}

int WFetchRenderer::CursorX() const
{
	return _cursorX;
}

int WFetchRenderer::CursorY() const
{
	return _cursorY;
}

void WFetchRenderer::Clear()
{
	auto&& values = _values.get();
	if (values == nullptr) return;

	ZeroMemory(values, this->GetValuesSize());
	auto size = _columns * _rows;
	for (int i = 0; i < size; i++)
	{
		auto&& info = _infos[i];
		info.BackgroundColor(-1);
		info.ForegroundColor(-1);
	}
}

void WFetchRenderer::MoveTo(int x, int y)
{
	_cursorX = x;
	_cursorY = y;
}

void WFetchRenderer::MoveToNextCharacter()
{
	auto x = _cursorX + 1;
	if (x >= _columns)
	{
		x = 0;
		auto y = _cursorY + 1;
		if (y >= _rows) 
		{
			y = 0;
		}
		_cursorY = y;
	}
	_cursorX = x;
}

void WFetchRenderer::MoveToLineBegin()
{
	_cursorX = 0;
}

void WFetchRenderer::MoveToNextLine()
{
	auto y = _cursorY + 1;
	if (y >= _rows)
	{
		y = 0;
	}
	_cursorY = y;
}

TCHAR WFetchRenderer::ReadChar()
{
	auto info = this->GetCurrentCharInfo();
	if (info == nullptr) return 0;
	return info->Read();
}

void WFetchRenderer::WriteChar(TCHAR value)
{
	auto info = this->GetCurrentCharInfo();
	if (info == nullptr) return;
	info->Write(value);
}

void WFetchRenderer::WriteString(const TCHAR* value)
{
	auto ch = *value;
	while (ch != 0)
	{
		if (ch == _T('\n'))
		{
			this->MoveToNextLine();
			this->MoveToLineBegin();
		}
		else
		{
			this->WriteChar(ch);
			this->MoveToNextCharacter();
		}

		value++;
		ch = *value;
	}
}

void WFetchRenderer::WriteBlockString(const TCHAR* value)
{
	auto ch = *value;
	auto x = this->CursorX();
	while (ch != 0)
	{	
		if (ch == _T('\n'))
		{
			this->MoveToNextLine();
			this->MoveTo(x, _cursorY);
		}
		else
		{
			this->WriteChar(ch);
			this->MoveToNextCharacter();
		}

		value++;
		ch = *value;
	}
}

void WFetchRenderer::SetForegroundColor(int8_t c)
{
	auto info = this->GetCurrentCharInfo();
	if (info == nullptr) return;
	info->ForegroundColor(c);
}

void WFetchRenderer::SetBackgroundColor(int8_t c)
{
	auto info = this->GetCurrentCharInfo();
	if (info == nullptr) return;
	info->BackgroundColor(c);
}

void WFetchRenderer::ResetColors()
{
	auto info = this->GetCurrentCharInfo();
	if (info == nullptr) return;
	info->BackgroundColor(-1);
	info->ForegroundColor(-1);
}

WFetchCharInfo* const& WFetchRenderer::GetCurrentCharInfo() const
{
	auto x = _cursorX;
	auto y = _cursorY;
	if (!this->IsCoordsWithinRange(x, y)) return nullptr;
	auto&& infos = _infos;
	return &infos[this->GetIndexFromCoords(x, y)];
}

int WFetchRenderer::GetValuesSize()
{
	return sizeof(TCHAR) * _columns * _rows;
}

int WFetchRenderer::GetIndexFromCoords(int x, int y) const
{
	return y * _columns + x;
}

bool WFetchRenderer::IsCoordsWithinRange(int x, int y) const
{
	if (x < 0 || x >= _columns) return false;
	if (y < 0 || y >= _rows) return false;
	return true;
}

void WFetchRenderer::RenderToDebug()
{
	auto rows = _rows;
	auto cols = _columns;

	auto values = _values.get();
	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			auto v = *values; values++;
			if (v == 0)
			{
				OutputDebugString(L" ");
				continue;
			}

			wstringstream ss;
			ss << v;
			OutputDebugString(ss.str().c_str());
			
		}
		OutputDebugString(L"\n");
	}
}

void WFetchRenderer::RenderToConsole()
{
	auto gaps = 0;
	bool hasContent = false;
	auto rows = _rows;
	auto cols = _columns;

	for (int r = 0; r < rows; r++)
	{
		if (this->IsRowEmpty(r))
		{
			gaps++;
			continue;
		}

		//fill in gaps
		while (gaps > 0)
		{
			wcout << endl;
			gaps--;
		}

		auto o = r * cols;
		auto leadings = 0;
		for (int c = 0; c < cols; c++)
		{
			auto&& info = _infos[o++];
			auto v = info.Read();
			if (v == 0)
			{
				leadings++;
				continue;
			}
			
			while (leadings > 0)
			{
				wcout << L' ';
				leadings--;
			}

			wcout << v;
		}

		wcout << endl;
	}

}


bool WFetchRenderer::IsRowEmpty(int r) const
{
	auto columns = _columns;
	auto offset = r * columns;
	auto&& values = _values.get();
	for (int i = 0; i < columns; i++)
	{
		if (values[offset] != 0) return false;
		offset++;
	}
	return true;
}
