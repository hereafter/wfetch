#pragma once
#include <WbemIdl.h>

using namespace std;
using namespace winrt;

class WInfoFetcher
{
public:
	WInfoFetcher();
	~WInfoFetcher();

public:
	wstring Title();
	wstring Underline(int count=0);

	wstring Distro();
	wstring Model();
	wstring Kernel();
	wstring Uptime();
	wstring Packages();
	wstring Shell();
	wstring Resolution();
	wstring Term();
	wstring Term_font();
	wstring Cpu();
	wstring Gpu();
	wstring Memory();

	wstring Battery();
	wstring Font();
	wstring Disk();
	wstring Local_ip();
	wstring Public_ip();
	wstring Users();
	wstring Locale();

	wstring Cols();

public:
	HRESULT Initialize();
	HRESULT QueryInstanceProperties(
		const TCHAR* className,
		vector<wstring> const& names,
		vector<wstring>& values
	);

private:
	
	wstring UserName();
	wstring HostName();


private:
	com_ptr<IWbemServices> _wbemServices;
};
