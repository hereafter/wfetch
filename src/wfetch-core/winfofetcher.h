#pragma once
#include <WbemIdl.h>
#include <atlbase.h>

#include "WFetchRenderBuffer.h"

using namespace std;
using namespace winrt;

enum class WFetchSupportedOS: int
{
	Windows10=0,
	Windows11=1,
	WindowsOthers=255,
};

class WInfoFetcher
{
public:
	WInfoFetcher();
	~WInfoFetcher();

public:
	wstring Title();
	wstring Underline(int count = 0);

	wstring Logo();

	wstring Distro();
	wstring Model();
	wstring Kernel();
	wstring Uptime();
	wstring Packages();
	wstring Resolution();
	wstring Shell();
	wstring ShellFont();
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

	wstring Colors();

public:
	HRESULT Initialize();
	HRESULT QueryInstanceProperties(
		const TCHAR* className,
		vector<wstring> const& names, 
		vector<CComVariant>& values
	);

public: //render to console
	void RenderToConsole();
	

private:
	wstring UserName();
	wstring HostName();

private:
	void DetectCurrentOS();
	void LoadColorProfiles();

	void FillStringValues(wstringstream& ss, 
		vector<CComVariant> const& values,
		const TCHAR* seperator = L" ");
	void FillLabelValueLine(wstringstream& ss,
		const TCHAR* label,
		const TCHAR* value);

	void FillStringValues(wstringstream& ss, 
		vector<wstring> const& values, 
		const TCHAR* seperator=L" ");
	
	int Execute(const TCHAR* cmd, const TCHAR* args, wstring& output);

	wstring FormatDiskSize(int64_t size, int precison=0);
	wstring FormatMemorySize(int64_t size);
	
	wstring GetFileVersion(const TCHAR* file);
	wstring GetPowershellVersion(const TCHAR* shell);
	wstring GetWslVersion(const TCHAR* shell);



private:
	com_ptr<IWbemServices> _wbemServices;
	WFetchRenderBuffer _renderBuffer;
	WFetchSupportedOS _currentOS;
};
