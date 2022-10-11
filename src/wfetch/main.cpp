#include "pch.h"
#include "winfofetcher.h"
#include <regex>
#include <map>

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace std;

void format_colors(wstring& text);

int main()
{
    init_apartment();

    WInfoFetcher fetcher;
    wstringstream ss;
    auto logo = fetcher.Logo();
    format_colors(logo);
    ss << logo.c_str() << endl;

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
    
    auto text = ss.str();
    format_colors(text);
    wprintf(text.c_str());

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