#include "pch.h"
#include "WFetchRenderBuffer.h"
#include <iostream>
#include <sstream>
#include <atlstr.h>

WFetchRenderBuffer::WFetchRenderBuffer():
	WFetchRenderBuffer(255, 100)
{}

WFetchRenderBuffer::WFetchRenderBuffer(int cols, int rows):
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

	//1 blue 2 green 4 red
	map<wstring, int> colors1 = 
	{
		{L"c0", 0},  {L"c1", 1},  {L"c2", 2},  {L"c3", 6},
		{L"c4", 4},  {L"c5", 5},  {L"c6", 6},  {L"c7", 7},
		{L"c8", 8},  {L"c9", 9},  {L"ca", 10}, {L"cb", 11}, 
		{L"cc", 12}, {L"cd", 13}, {L"ce", 14}, {L"cf", 15}
	};

	map<wstring, int> colors2 =
	{
		{L"b0",        0},  {L"b1", 0x0010+1},  {L"b2", 0x0010+2},  {L"b3", 0x0010+3},
		{L"b4", 0x0010+4},  {L"b5", 0x0010+5},  {L"b6", 0x0010+6},  {L"b7", 0x0010+7},
		{L"b8", 0x0010+8},  {L"b9", 0x0010+9},  {L"ba", 0x0010+10}, {L"bb", 0x0010+11},
		{L"bc", 0x0010+12}, {L"bd", 0x0010+13}, {L"be", 0x0010+14}, {L"bf", 0x0010+15}
	};

	_foregroundColors = colors1;
	_backgroundColors = colors2;

	auto h = ::GetStdHandle(STD_OUTPUT_HANDLE);
	if (h != nullptr)
	{
		CONSOLE_SCREEN_BUFFER_INFO info = { 0 };
		::GetConsoleScreenBufferInfo(h, &info);
		_defaultColor = info.wAttributes;
	}
	else
	{
		_defaultColor = 0;
	}

}

WFetchRenderBuffer::~WFetchRenderBuffer()
{}

int WFetchRenderBuffer::CursorX() const
{
	return _cursorX;
}

int WFetchRenderBuffer::CursorY() const
{
	return _cursorY;
}

void WFetchRenderBuffer::Clear()
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

void WFetchRenderBuffer::MoveTo(int x, int y)
{
	_cursorX = x;
	_cursorY = y;
}

void WFetchRenderBuffer::MoveToNextCharacter()
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

void WFetchRenderBuffer::MoveToLineBegin()
{
	_cursorX = 0;
}

void WFetchRenderBuffer::MoveToNextLine()
{
	auto y = _cursorY + 1;
	if (y >= _rows)
	{
		y = 0;
	}
	_cursorY = y;
}

TCHAR WFetchRenderBuffer::ReadChar()
{
	auto info = this->GetCurrentCharInfo();
	if (info == nullptr) return 0;
	return info->Read();
}

void WFetchRenderBuffer::WriteChar(TCHAR value)
{
	auto info = this->GetCurrentCharInfo();
	if (info == nullptr) return;
	info->Write(value);
}

void WFetchRenderBuffer::WriteString(const TCHAR* value)
{
	auto ch = *value;
	wstringstream ss;
	bool controls = false;
	bool dollar = false;

	wstring colors;

	while (ch != 0)
	{
		if (controls)
		{
			if (ch == _T('}'))
			{
				controls = false;
				dollar = false;
				colors = ss.str();
				this->ProcessColors(colors);
				ss.str(L"");
				goto NEXT;
			}
			
			ss << ch;
			goto NEXT;
		}
		

		if (ch == _T('$'))
		{
			dollar = true;
			ss << ch;
			goto NEXT;
		}
		else if (ch == _T('{'))
		{
			if (dollar)
			{
				controls = true;
				ss.str(L"");
				goto NEXT;
			}
		}


		if (dollar)
		{
			dollar = false;
		}

		if (ch == _T('\n'))
		{
			this->MoveToNextLine();
			this->MoveToLineBegin();
			if (!colors.empty())
			{
				this->ProcessColors(colors);
			}
		}
		else
		{
			this->WriteChar(ch);
			this->MoveToNextCharacter();
		}
		

	NEXT:
		value++;
		ch = *value;
	}
}

void WFetchRenderBuffer::WriteBlockString(const TCHAR* value)
{
	auto ch = *value;
	auto x = this->CursorX();

	wstringstream ss;
	bool controls = false;
	bool dollar = false;

	wstring colors;

	while (ch != 0)
	{
		if (controls)
		{
			if (ch == _T('}'))
			{
				controls = false;
				dollar = false;
				colors = ss.str();
				this->ProcessColors(colors);

				ss.str(L"");
				goto NEXT;
			}

			ss << ch;
			goto NEXT;
		}


		if (ch == _T('$'))
		{
			dollar = true;
			ss << ch;
			goto NEXT;
		}
		else if (ch == _T('{'))
		{
			if (dollar)
			{
				controls = true;
				ss.str(L"");
				goto NEXT;
			}
		}


		if (dollar)
		{
			dollar = false;
		}

		if (ch == _T('\n'))
		{
			this->MoveToNextLine();
			this->MoveTo(x, _cursorY);
			if (!colors.empty())
			{
				this->ProcessColors(colors);
			}
		}
		else
		{
			this->WriteChar(ch);
			this->MoveToNextCharacter();
		}


	NEXT:
		value++;
		ch = *value;
	}
}

void WFetchRenderBuffer::SetForegroundColor(int8_t c)
{
	auto info = this->GetCurrentCharInfo();
	if (info == nullptr) return;
	info->ForegroundColor(c);
}

void WFetchRenderBuffer::SetBackgroundColor(int8_t c)
{
	auto info = this->GetCurrentCharInfo();
	if (info == nullptr) return;
	info->BackgroundColor(c);
}

void WFetchRenderBuffer::ResetColors()
{
	auto info = this->GetCurrentCharInfo();
	if (info == nullptr) return;
	info->BackgroundColor(-1);
	info->ForegroundColor(-1);
}

void WFetchRenderBuffer::SetColors(vector<int>& fcs, vector<int>& bcs)
{
	map<wstring, int> colors1;
	map<wstring, int> colors2;

	int index = 0;
	wstringstream ss;
	for (auto c : fcs)
	{
		ss.str(L""); ss << "c"<< index++;
		colors1[ss.str()] = c;
	}

	index = 0;
	for (auto c : bcs)
	{
		ss.str(L""); ss << "b" << index++;
		colors2[ss.str()] = c;
	}

	_foregroundColors = colors1;
	_backgroundColors = colors2;
}

void WFetchRenderBuffer::ProcessColors(wstring const& controls)
{
	auto info = this->GetCurrentCharInfo();
	if (info == nullptr) return;

	auto&& k = controls;
	auto&& colors1 = _foregroundColors;
	auto v1 = colors1.find(k);
	if (v1 != colors1.end())
	{
		info->ForegroundColor(v1->second);
	}

	auto&& colors2 = _backgroundColors;
	auto v2 = colors2.find(k);
	if (v2 != colors2.end()) 
	{
		info->BackgroundColor(v2->second);
	}
}

WFetchCharInfo* const WFetchRenderBuffer::GetCurrentCharInfo() const
{
	auto x = _cursorX;
	auto y = _cursorY;
	if (!this->IsCoordsWithinRange(x, y)) return nullptr;
	auto p=&_infos[this->GetIndexFromCoords(x, y)];
	return p;
}

int WFetchRenderBuffer::GetValuesSize()
{
	return sizeof(TCHAR) * _columns * _rows;
}

int WFetchRenderBuffer::GetIndexFromCoords(int x, int y) const
{
	return y * _columns + x;
}

bool WFetchRenderBuffer::IsCoordsWithinRange(int x, int y) const
{
	if (x < 0 || x >= _columns) return false;
	if (y < 0 || y >= _rows) return false;
	return true;
}

void WFetchRenderBuffer::RenderToDebug()
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

void WFetchRenderBuffer::RenderToConsole()
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
			cout << endl;
			gaps--;
		}

		auto o = r * cols;
		auto leadings = 0;
		TCHAR tmp[2] = { 0 };
		for (int c = 0; c < cols; c++)
		{
			auto&& info = _infos[o++];
			auto v = info.Read();

			if (info.IsColorChanging())
			{
				auto fc = info.ForegroundColor();
				auto bc = info.BackgroundColor();
				if (bc <= 0)
				{
					bc = _defaultColor & 0xf0;
				}

				if (fc <= 0)
				{
					fc = _defaultColor & 0x0f;
				}

				auto c = 0;
				if (fc > 0) c += fc;
				if (bc > 0) c += bc;
				auto h = ::GetStdHandle(STD_OUTPUT_HANDLE);
				SetConsoleTextAttribute(h, c);
			}

			if (v == 0)
			{
				leadings++;
				continue;
			}
			
			while (leadings > 0)
			{
				cout << ' ';
				leadings--;
			}			

			tmp[0] = v;
			cout << CW2A(tmp, CP_UTF8).m_psz;
		}

		cout << endl;
	}

	auto h = ::GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(h, _defaultColor);
}


bool WFetchRenderBuffer::IsRowEmpty(int r) const
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
