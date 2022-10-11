#pragma once
#include <WbemIdl.h>
#include <atlbase.h>

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
	wstring TermFont();
	wstring Cpu();
	wstring Gpu();
	wstring Memory();

	wstring Battery();
	wstring Font();
	wstring Disk();
	wstring LocalIp();
	wstring PublicIp();
	wstring Users();
	wstring Locale();

	wstring Cols();

public:
	HRESULT Initialize();
	HRESULT QueryInstanceProperties(
		const TCHAR* className,
		vector<wstring> const& names,
		vector<CComVariant>& values
	);

private:
	
	wstring UserName();
	wstring HostName();

private:
	void FillStringValues(wstringstream& ss, 
		vector<CComVariant> const& values,
		const TCHAR* seperator = L" ");
	void FillStringValues(wstringstream& ss, 
		vector<wstring> const& values, 
		const TCHAR* seperator=L" ");
	
	int Execute(const TCHAR* cmd, const TCHAR* args, vector<wstring>& outputs);

	wstring FormatDiskSize(int64_t size);
	wstring FormatMemorySize(int64_t size);
private:
	com_ptr<IWbemServices> _wbemServices;
};
