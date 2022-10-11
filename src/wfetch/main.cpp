#include "pch.h"
#include "winfofetcher.h"
#include <regex>
#include <map>
#include <concrt.h>
using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace std;

void format_colors(wstring& text);
void remove_colors(wstring& text);
int text_width(wstring const& text);
int text_height(wstring const& text);
void output_text(wstring& const text, HANDLE h, int x, int y);

int main()
{
    init_apartment();

    WInfoFetcher fetcher;
    wstringstream ss;
    auto logo = fetcher.Logo();
    format_colors(logo);
    ss << logo.c_str() << endl;

    auto logoWidth = text_width(logo);
    auto logoHeight = text_height(logo);

    auto hOutput=::GetStdHandle(STD_OUTPUT_HANDLE);
    
    CONSOLE_SCREEN_BUFFER_INFO info = { 0 };
    ::GetConsoleScreenBufferInfo(hOutput, &info);
    INT x = 0; INT y = 0;
    x = info.dwCursorPosition.X;
    y = info.dwCursorPosition.Y;

    output_text(logo, hOutput, x, y);
    
    ss.str(L"");
    ss << "${c9}"<< fetcher.Title().c_str() << endl;
    ss << "${c7}" << fetcher.Underline().c_str() << endl;
    ss << "${c9}OS:${c7} " <<  fetcher.Distro().c_str() << endl;
    ss << "${c9}Kernel:${c7} " << fetcher.Kernel().c_str() << endl;
    ss << "${c9}Model:${c7} " << fetcher.Model().c_str() << endl;
    ss << "${c9}Uptime:${c7} " << fetcher.Uptime().c_str() << endl;
    ss << "${c9}Packages:${c7} " << fetcher.Packages().c_str() << endl;
    ss << "${c9}Shell:${c7} " << fetcher.Shell().c_str() << endl;
    ss << "${c9}Resolution:${c7} " << fetcher.Resolution().c_str() << endl;
    ss << "${c9}CPU:${c7} " << fetcher.Cpu().c_str() << endl;
    ss << "${c9}GPU:${c7} " << fetcher.Gpu().c_str() << endl;
    ss << "${c9}Memory:${c7} " << fetcher.Memory().c_str() << endl;
    ss << "${c9}Font:${c7} " << fetcher.Font().c_str() << endl;
    ss << fetcher.Disk();

    ss << fetcher.Colors();    
    auto text = ss.str();
    format_colors(text);

    x += 3 + logoWidth;
    output_text(text, hOutput, x, y);
}


void format_colors(wstring& text)
{
    static map<wstring, wstring> const  colors =
    {
        {L"\\$\\{c0\\}", L"\033[30m"},
        {L"\\$\\{c1\\}", L"\033[31m"},
        {L"\\$\\{c2\\}", L"\033[32m"},
        {L"\\$\\{c3\\}", L"\033[33m"},
        {L"\\$\\{c4\\}", L"\033[34m"},
        {L"\\$\\{c5\\}", L"\033[35m"},
        {L"\\$\\{c6\\}", L"\033[36m"},
        {L"\\$\\{c7\\}", L"\033[37m"},
        {L"\\$\\{c8\\}", L"\033[90m"},
        {L"\\$\\{c9\\}", L"\033[91m"},
        {L"\\$\\{ca\\}", L"\033[92m"},
        {L"\\$\\{cb\\}", L"\033[93m"},
        {L"\\$\\{cc\\}", L"\033[94m"},
        {L"\\$\\{cd\\}", L"\033[95m"},
        {L"\\$\\{ce\\}", L"\033[96m"},
        {L"\\$\\{cf\\}", L"\033[97m"},
    };

    for (auto&& p : colors)
    {
        text = regex_replace(text, wregex(p.first), p.second);
    }
}

void remove_colors(wstring& text)
{
    static map<wstring, wstring> const templates =
    {
        {L"\\$\\{c0\\}", L""},
        {L"\\$\\{c1\\}", L""},
        {L"\\$\\{c2\\}", L""},
        {L"\\$\\{c3\\}", L""},
        {L"\\$\\{c4\\}", L""},
        {L"\\$\\{c5\\}", L""},
        {L"\\$\\{c6\\}", L""},
        {L"\\$\\{c7\\}", L""},
        {L"\\$\\{c8\\}", L""},
        {L"\\$\\{c9\\}", L""},
        {L"\\$\\{ca\\}", L""},
        {L"\\$\\{cb\\}", L""},
        {L"\\$\\{cc\\}", L""},
        {L"\\$\\{cd\\}", L""},
        {L"\\$\\{ce\\}", L""},
        {L"\\$\\{cf\\}", L""},
    };

    for (auto&& p : templates)
    {
        text = regex_replace(text, wregex(p.first), p.second);
    }
}


int text_width(wstring const& text)
{
    auto temp = text;
    remove_colors(temp);
    wstringstream ss(temp);
    wstring line;
    int length = 0;
    while (getline(ss, line))
    {
        length=max(length, line.length());
    }
    return length;
}

int text_height(wstring const& text)
{
    auto temp = text;
    remove_colors(temp);
    wstringstream ss(temp);
    wstring line;
    int height = 0;
    while (getline(ss, line))
    {
        height++;
    }
    return height;
}

void output_text(wstring& const text, HANDLE h, int x, int y)
{
    wstringstream ss(text);
    wstring line;
    int height = 0;
    while (getline(ss, line))
    {
        height++;
        ::SetConsoleCursorPosition(h, { (SHORT)x, (SHORT)y });
        y++;
        wprintf(line.c_str());
    }
}