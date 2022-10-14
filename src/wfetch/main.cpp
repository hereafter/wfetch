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
    ss << "${c1}"<< fetcher.Title().c_str() << endl;
    ss << "${c0}" << fetcher.Underline().c_str() << endl;
    ss << "${c1}OS:${c0} " <<  fetcher.Distro().c_str() << endl;
    ss << "${c1}Kernel:${c0} " << fetcher.Kernel().c_str() << endl;
    ss << "${c1}Model:${c0} " << fetcher.Model().c_str() << endl;
    ss << "${c1}Uptime:${c0} " << fetcher.Uptime().c_str() << endl;
    ss << "${c1}Packages:${c0} " << fetcher.Packages().c_str() << endl;
    ss << "${c1}Shell:${c0} " << fetcher.Shell().c_str() << endl;
    ss << "${c1}Resolution:${c0} " << fetcher.Resolution().c_str() << endl;
    ss << "${c1}CPU:${c0} " << fetcher.Cpu().c_str() << endl;
    ss << "${c1}GPU:${c0} " << fetcher.Gpu().c_str() << endl;
    ss << "${c1}Memory:${c0} " << fetcher.Memory().c_str() << endl;
    ss << "${c1}Font:${c0} " << fetcher.Font().c_str() << endl;
    ss << fetcher.Disk();

    auto info = ss.str();
    renderer.MoveTo(36, 0);
    renderer.WriteBlockString(info.c_str());

    renderer.RenderToDebug();
    renderer.RenderToConsole();

}

