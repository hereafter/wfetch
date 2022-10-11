#include "pch.h"
#include "winfofetcher.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;

int main()
{
    init_apartment();
    HRESULT hr=::CoInitializeSecurity(nullptr, -1, nullptr, nullptr,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        nullptr,
        EOAC_NONE, nullptr);
    if (FAILED(hr)) return -1;

    WInfoFetcher fetcher;
    wstringstream ss;
    ss << fetcher.Title().c_str() << endl;
    ss << fetcher.Underline().c_str() << endl;
    ss << "OS: " <<  fetcher.Distro().c_str() << endl;
    ss << "Kernel: " << fetcher.Kernel().c_str() << endl;
    ss << "Model: " << fetcher.Model().c_str() << endl;
    ss << "Uptime: " << fetcher.Uptime().c_str() << endl;
    ss << "Packages: " << fetcher.Packages().c_str() << endl;
    ss << "Shell: " << fetcher.Shell().c_str() << endl;
    ss << "Resolution: " << fetcher.Resolution().c_str() << endl;
    ss << "CPU: " << fetcher.Cpu().c_str() << endl;
    ss << "GPU: " << fetcher.Gpu().c_str() << endl;
    ss << "Memory: " << fetcher.Memory().c_str() << endl;
    
    wprintf(ss.str().c_str());

}
