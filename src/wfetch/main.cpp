#include "pch.h"

#include <regex>
#include <map>
#include <concrt.h>

#include "winfofetcher.h"
#include "wfetchrenderer.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace std;

int main()
{
    init_apartment();

    WInfoFetcher fetcher;
    WFetchRenderer renderer;
    wstringstream ss;
    auto logo = fetcher.Logo();
    ss << logo.c_str() << endl;

    renderer.MoveTo(0, 0);
    renderer.WriteString(logo.c_str());

    
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

    auto info = ss.str();
    renderer.MoveTo(30, 0);
    renderer.WriteBlockString(info.c_str());

    renderer.RenderToConsole();

}

