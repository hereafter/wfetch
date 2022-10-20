#include "pch.h"
#include "WInfoFetcher.h"
#include <iostream>
#include <conio.h>

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace std;

int main()
{

    init_apartment();
 
    WInfoFetcher fetcher;

    auto shell = fetcher.Shell();
	auto noshell = (shell.find(L"dllhost") == 0 || shell.find(L"explorer") == 0);
    
    if (noshell)
    {
        SetConsoleTitle(L"wfetch");
    }


    fetcher.RenderToConsole();

    
    if (noshell)
    {
        cout << endl << "Press any key to continue...";
        while (!_kbhit())
        {
            Sleep(10);
        }
    }    
    return 0;
}

