#include "pch.h"
#include "winfofetcher.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;

int main()
{
    init_apartment();

    WInfoFetcher fetcher;
    wstringstream ss;
    ss << fetcher.Title().c_str() << endl;
    ss << fetcher.Underline().c_str() << endl;
    
    wprintf(ss.str().c_str());

}
