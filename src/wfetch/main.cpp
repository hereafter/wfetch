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
    ss << "\033[91m"<< fetcher.Title().c_str() << endl;
    ss << "\033[0m" << fetcher.Underline().c_str() << endl;
    ss << "\033[91mOS:\033[0m " <<  fetcher.Distro().c_str() << endl;
    ss << "\033[91mKernel:\033[0m " << fetcher.Kernel().c_str() << endl;
    ss << "\033[91mModel:\033[0m " << fetcher.Model().c_str() << endl;
    ss << "\033[91mUptime:\033[0m " << fetcher.Uptime().c_str() << endl;
    ss << "\033[91mPackages:\033[0m " << fetcher.Packages().c_str() << endl;
    ss << "\033[91mShell:\033[0m " << fetcher.Shell().c_str() << endl;
    ss << "\033[91mResolution:\033[0m " << fetcher.Resolution().c_str() << endl;
    ss << "\033[91mCPU:\033[0m " << fetcher.Cpu().c_str() << endl;
    ss << "\033[91mGPU:\033[0m " << fetcher.Gpu().c_str() << endl;
    ss << "\033[91mMemory:\033[0m " << fetcher.Memory().c_str() << endl;
    ss << "\033[91mFont:\033[0m " << fetcher.Font().c_str() << endl;
    
    wprintf(ss.str().c_str());

}
