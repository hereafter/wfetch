#include "pch.h"

#include <regex>
#include <map>
#include <concrt.h>

#include "WInfoFetcher.h"

#include <stdio.h>


using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace std;

int main()
{
    init_apartment();
 
    WInfoFetcher fetcher;
    fetcher.RenderToConsole();

    getc(stdin);
    
    return 0;
}

