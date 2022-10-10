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
    
    wprintf(ss.str().c_str());

}
