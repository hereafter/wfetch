#include "pch.h"
#include "WInfoFetcher.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace std;

int main()
{
    init_apartment();
 
    WInfoFetcher fetcher;
    fetcher.RenderToConsole();
    
    return 0;
}

