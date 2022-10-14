#include "pch.h"
#include "winfofetcher.h"
#include <chrono>
#include <sstream>

using namespace std;
using namespace std::chrono;

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::System;

#pragma region Logos
constexpr const TCHAR* kWindows11LogoASCII = L""
"${c1}\n"
"################  ################\n"
"################  ################\n"
"################  ################\n"
"################  ################\n"
"################  ################\n"
"################  ################\n"
"################  ################\n"
"\n"
"################  ################\n"
"################  ################\n"
"################  ################\n"
"################  ################\n"
"################  ################\n"
"################  ################\n"
"################  ################\n";


constexpr const TCHAR* kWindows10LogoASCII = L""
"${c1}                                ..,\n"
"                    ....,,:;+ccllll\n"
"      ...,,+:;  cllllllllllllllllll\n"
",cclllllllllll  lllllllllllllllllll\n"
"llllllllllllll  lllllllllllllllllll\n"
"llllllllllllll  lllllllllllllllllll\n"
"llllllllllllll  lllllllllllllllllll\n"
"llllllllllllll  lllllllllllllllllll\n"
"llllllllllllll  lllllllllllllllllll\n"
"\n"
"llllllllllllll  lllllllllllllllllll\n"
"llllllllllllll  lllllllllllllllllll\n"
"llllllllllllll  lllllllllllllllllll\n"
"llllllllllllll  lllllllllllllllllll\n"
"llllllllllllll  lllllllllllllllllll\n"
"`'ccllllllllll  lllllllllllllllllll\n"
"       `' \\*::  :ccllllllllllllllll\n"
"                       ````''*::cll\n"
"                                 ``\n";

constexpr const TCHAR* kWindowsLogoASCII=L""
"${c1}        ,.=:!!t3Z3z.,\n"
"       :tt:::tt333EE3\n"
"${c1}       Et:::ztt33EEEL${c2} @Ee.,      ..,\n"
"${c1}      ;tt:::tt333EE7${c2} ;EEEEEEttttt33#\n"
"${c1}     :Et:::zt333EEQ.${c2} $EEEEEttttt33QL\n"
"${c1}     it::::tt333EEF${c2} @EEEEEEttttt33F\n"
"${c1}    ;3=*^```\"*4EEV${c2} :EEEEEEttttt33@.\n"
"${c3}    ,.=::::!t=., ${c1}`${c2} @EEEEEEtttz33QF\n"
"${c3}   ;::::::::zt33)${c2}   \"4EEEtttji3P*\n"
"${c3}  :t::::::::tt33.${c4}:Z3z..${c2}  ``${c4} ,..g.\n"
"${c3}  i::::::::zt33F${c4} AEEEtttt::::ztF\n"
"${c3} ;:::::::::t33V${c4} ;EEEttttt::::t3\n"
"${c3} E::::::::zt33L${c4} @EEEtttt::::z3F\n"
"${c3}{3=*^```\"*4E3)${c4} ;EEEtttt:::::tZ`\n"
"${c3}             `${c4} :EEEEtttt::::z7\n"
"                 \"VEzjt:;;z>*`\n";

#pragma endregion

WInfoFetcher::WInfoFetcher()
{
	HRESULT hr=this->Initialize();
	if (FAILED(hr)) winrt::throw_hresult({ hr });
}
WInfoFetcher::~WInfoFetcher()
{}

wstring WInfoFetcher::UserName()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	DWORD bufferSize = MAX_PATH;
	::GetUserName(buffer, &bufferSize);
	return buffer;
}
wstring WInfoFetcher::HostName()
{
	TCHAR buffer[MAX_PATH] = { 0 };
	DWORD bufferSize = MAX_PATH;
	::GetComputerName(buffer, &bufferSize);
	return buffer;
}
wstring WInfoFetcher::Title()
{
	wstringstream ss;
	ss << UserName().c_str();
	auto domain = HostName();
	if (!domain.empty())
	{
		ss << "@" << domain.c_str();
	}
	return ss.str();
}
wstring WInfoFetcher::Underline(int count)
{
	if (count <= 0)
	{
		count = this->Title().length();
	}

	wstringstream ss;
	for (int i = 0; i < count; i++)
	{
		ss << "-";
	}
	return ss.str();
}

wstring WInfoFetcher::Logo()
{
	return kWindows11LogoASCII;
}
wstring WInfoFetcher::Distro()
{
	wstringstream ss;
	vector<CComVariant> values;
	HRESULT hr = NOERROR;
	hr = this->QueryInstanceProperties(
		L"Win32_OperatingSystem", 
		{ L"Caption", L"OSArchitecture" },
		values);
	if (FAILED(hr)) return ss.str();

	this->FillStringValues(ss, values);
	return ss.str();
}
wstring WInfoFetcher::Model()
{
	wstringstream ss;
	vector<CComVariant> values = { };

	HRESULT hr = NOERROR;
	hr = this->QueryInstanceProperties(
		L"Win32_ComputerSystem", 
		{L"Manufacturer", L"Model"},
		values);
	if (FAILED(hr)) return ss.str();
	this->FillStringValues(ss, values);
	return ss.str();
}
wstring WInfoFetcher::Kernel()
{
	wstringstream ss;
	vector<CComVariant> values = { };
	HRESULT hr = NOERROR;
	hr = this->QueryInstanceProperties(
		L"Win32_OperatingSystem",
		{L"Version"}, 
		values);
	if (FAILED(hr)) return ss.str();

	this->FillStringValues(ss, values);
	return ss.str();
}
wstring WInfoFetcher::Uptime()
{
	wstringstream ss;

	vector<CComVariant> values;
	HRESULT hr = this->QueryInstanceProperties(
		L"Win32_OperatingSystem",
		{L"LastBootUpTime", L"LocalDateTime"},
		values
	);
	if (FAILED(hr)) return ss.str();

	wistringstream s1(values[0].bstrVal);
	wistringstream s2(values[1].bstrVal);
	tm t1{};
	tm t2{};
	s1 >> get_time(&t1, L"%Y%m%d%H%M%S");
	s2 >> get_time(&t2, L"%Y%m%d%H%M%S");
	
	auto span=mktime(&t2) - mktime(&t1);

	int days = static_cast<int>(span / 3600 / 24);
	int hours = static_cast<int>(span % (3600 * 24) / 3600);
	int mins = static_cast<int>(span % (3600) / 60);


	bool empty = true;
	if (days > 0) 
	{
		ss << days;
		if (days != 1)
		{
			ss << " days";
		}
		else
		{
			ss << " day";
		}
		empty = false;
	}

	if (!empty || hours > 0)
	{
		if (!empty) ss << ", ";
		ss << hours;
		if (hours != 1)
		{
			ss << " hours";
		}
		else
		{
			ss << " hour";
		}
		empty = false;
	}

	if (!empty || mins > 0)
	{
		if (!empty) ss << ", ";
		ss << mins;
		if (mins != 1)
		{
			ss << " mins";
		}
		else
		{
			ss << " min";
		}
		empty = false;
	}


	if (empty)
	{
		ss << "secs";
	}

	return ss.str();
}
wstring WInfoFetcher::Packages()
{
	wstringstream ss;

	vector<wstring> outputs;
	auto code=this->Execute(L"winget", L"list -s winget", outputs);
	if (code < 0) return ss.str();




	return ss.str();
}
wstring WInfoFetcher::Shell()
{
	wstringstream ss;
	return ss.str();
}
wstring WInfoFetcher::Resolution()
{
	vector<wstring> resolutions;
	::EnumDisplayMonitors(nullptr, nullptr,
		[](HMONITOR h, HDC dc, LPRECT lpRect, LPARAM lParam) {
			auto&& resolutions = *(vector<wstring>*)lParam;
			auto bounds = *lpRect;
			wstringstream ss;
			ss << (bounds.right - bounds.left) << "x";
			ss << (bounds.bottom - bounds.top);
			resolutions.push_back(ss.str());
			return TRUE;
		}, (LPARAM)(void*)&resolutions);

	wstringstream ss;
	this->FillStringValues(ss, resolutions, L", ");
	return ss.str();
}
wstring WInfoFetcher::Term()
{
	wstringstream ss;
	return ss.str();
}
wstring WInfoFetcher::TermFont()
{
	wstringstream ss;
	return ss.str();
}
wstring WInfoFetcher::Cpu()
{
	wstringstream ss;
	vector<CComVariant> values;
	HRESULT hr = this->QueryInstanceProperties(
		L"Win32_Processor",
		{L"Name"},
		values
	);
	if (FAILED(hr)) return ss.str();
	ss << values[0].bstrVal;
	return ss.str();
}
wstring WInfoFetcher::Gpu()
{
	wstringstream ss;
	vector<CComVariant> values;
	HRESULT hr = this->QueryInstanceProperties(
		L"Win32_DisplayConfiguration",
		{L"DeviceName"},
		values
	);
	if (FAILED(hr)) return ss.str();
	ss << values[0].bstrVal;
	return ss.str();
}
wstring WInfoFetcher::Memory()
{
	wstringstream ss;

	vector<CComVariant> values, values2;
	HRESULT hr = this->QueryInstanceProperties(
		L"Win32_OperatingSystem",
		{L"FreePhysicalMemory"},
		values
	);
	if (FAILED(hr)) return ss.str();
	hr = this->QueryInstanceProperties(
		L"Win32_ComputerSystem",
		{ L"TotalPhysicalMemory" },
		values2
	);
	if (FAILED(hr)) return ss.str();

	int64_t size1=0, size2=0;
	size1=stoll(values[0].bstrVal)*1024;
	size2=stoll(values2[0].bstrVal);
	auto size3 = size2 - size1;

	ss << this->FormatMemorySize(size3).c_str();
	ss << " / ";
	ss << this->FormatMemorySize(size2).c_str();

	return ss.str();
}

wstring WInfoFetcher::Battery()
{
	wstringstream ss;
	return ss.str();
}

wstring WInfoFetcher::Font()
{
	wstringstream ss;
	ss << "Segoe UI";
	return ss.str();
}

wstring WInfoFetcher::Disk()
{
	wstringstream ss;
	HRESULT hr = NOERROR;

	vector<CComVariant> values;
	hr = this->QueryInstanceProperties(L"Win32_LogicalDisk",
		{L"DeviceId", L"FreeSpace", L"Size"},
		values);
	if (FAILED(hr)) return ss.str();

	auto count = values.size() / 3;
	
	for (size_t i = 0; i < count; i++)
	{
		int offset = i * 3;
		ss << "${c1}Disk " << values[offset].bstrVal;
		ss << " ${c0}";

		auto size1 = stoll(values[offset + 1].bstrVal);
		auto size2 = stoll(values[offset + 2].bstrVal);

		ss << this->FormatDiskSize(size1);
		ss << " / ";
		ss << this->FormatDiskSize(size2);

		ss << endl;
	}


	return ss.str();
}

wstring WInfoFetcher::LocalIp()
{
	wstringstream ss;
	return ss.str();
}

wstring WInfoFetcher::PublicIp()
{
	wstringstream ss;
	return ss.str();
}

wstring WInfoFetcher::Users()
{
	wstringstream ss;
	return ss.str();
}

wstring WInfoFetcher::Locale()
{
	wstringstream ss;
	return ss.str();
}

wstring WInfoFetcher::Colors()
{
	wstringstream ss;
	ss << (TCHAR)219;
	return ss.str();
}

HRESULT WInfoFetcher::Initialize()
{
	if (_wbemServices != nullptr) return S_FALSE;

	HRESULT hr = NOERROR;
	auto locator = create_instance<IWbemLocator>(CLSID_WbemLocator);
	
	hr=locator->ConnectServer(
		BSTR(L"ROOT\\CIMV2"),
		nullptr, nullptr, 0, NULL, 0, 0, _wbemServices.put());
	if (FAILED(hr)) goto BAIL;

	hr=::CoSetProxyBlanket(_wbemServices.get(), RPC_C_AUTHN_WINNT,
		RPC_C_AUTHZ_NONE, nullptr, RPC_C_AUTHN_LEVEL_CALL,
		RPC_C_IMP_LEVEL_IMPERSONATE, nullptr, EOAC_NONE);

	if (FAILED(hr)) goto BAIL;

BAIL:
	if (FAILED(hr))
	{
		_wbemServices = nullptr;
	}
	return hr;
}

HRESULT WInfoFetcher::QueryInstanceProperties(
	const TCHAR* className,
	vector<wstring> const& names,
	vector<CComVariant>& values)
{
	HRESULT hr = NOERROR;
	auto s = _wbemServices.get();
	if (s == nullptr) 
	{
		hr=this->Initialize();
		if (FAILED(hr)) return hr;
		s = _wbemServices.get();
	}

	com_ptr<IEnumWbemClassObject> objs;
	com_ptr<IWbemClassObject> obj;

	wstringstream qss;
	qss << "SELECT * FROM " << className;
	auto qs = qss.str();
	hr = s->ExecQuery(
		BSTR(L"WQL"),
		BSTR(qs.c_str()),
		WBEM_FLAG_RETURN_IMMEDIATELY | WBEM_FLAG_FORWARD_ONLY,
		nullptr,
		objs.put()
	);

	if (FAILED(hr)) return hr;

	ULONG uCode = 0;
	while (SUCCEEDED(objs->Next(WBEM_INFINITE, 1,
		obj.put(), &uCode)))
	{
		if (uCode == 0) break;

		CComVariant vt;
		for (auto&& n : names)
		{
			hr = obj->Get(n.c_str(), 0, &vt, 0, 0);
			if (SUCCEEDED(hr))
			{
				values.push_back(vt);
			}
		}
		vt.Clear();	
	}
	return hr;
}

int WInfoFetcher::Execute(const TCHAR* cmd, const TCHAR* args, vector<wstring>& outputs)
{

	return 0;
}

void WInfoFetcher::FillStringValues(wstringstream& ss, 
	vector<CComVariant> const& values,
	const TCHAR* seperator)
{

	vector<wstring> values2;
	for (auto&& v : values) { values2.push_back(v.bstrVal); }
	this->FillStringValues(ss, values2);
}

void WInfoFetcher::FillStringValues(wstringstream& ss, 
	vector<wstring> const& values,
	const TCHAR* seperator)
{
	for (auto&& v : values)
	{
		if (!v.empty())
		{
			if (ss.tellp() != 0)
			{
				ss << seperator;
			}
			ss << v.c_str();
		}
	}
}


wstring WInfoFetcher::FormatDiskSize(int64_t size, int precision)
{
	wstringstream ss;
	ss.precision(precision);
	ss << fixed;
	if (size < 1024)
	{
		ss << size << " Bytes";
	}
	else if (size >> 10 < 1024)
	{
		ss << (size / 1024.0f) << "K";
	}
	else if (size >> 20 < 1024)
	{
		ss << ((size >> 10) / 1024.0f) << "M";
	}
	else if (size >> 30 < 1024)
	{
		ss << ((size >> 20) / 1024.0f) << "G";
	}
	else if (size >> 40 < 1024)
	{
		ss << ((size >> 30) / 1024.0f) << "T";
	}
	else if (size >> 50 < 1024)
	{
		ss << ((size >> 40) / 1024.0f) << "P";
	}
	else
	{
		ss << ((size >> 50) / 1024.0f) << "E";
	}

	return ss.str();
}

wstring WInfoFetcher::FormatMemorySize(int64_t size)
{
	wstringstream ss;
	if (size < 1024)
	{
		ss << size << "Bytes";
	}
	else if (size >> 10 < 1024)
	{
		ss << (size / 1024) << "KiB";
	}
	else
	{
		ss << ((size >> 10) / 1024) << "MiB";
	}
	return ss.str();
}