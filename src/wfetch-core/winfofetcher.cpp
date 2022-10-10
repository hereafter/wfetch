#include "pch.h"
#include "winfofetcher.h"
#include <chrono>
#include <sstream>
#include <ShlObj.h>
#include <Shlwapi.h>
#include <shellapi.h>

using namespace std;
using namespace std::chrono;

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::System;

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
		ss << "=";
	}
	return ss.str();
}
wstring WInfoFetcher::Distro()
{
	wstringstream ss;

	vector<wstring> names = { L"Caption", L"OSArchitecture" };
	vector<CComVariant> values = { };

	HRESULT hr = NOERROR;
	hr = this->QueryInstanceProperties(L"Win32_OperatingSystem", names, values);
	if (SUCCEEDED(hr))
	{
		this->FillStringValues(ss, values);
	}
	return ss.str();
}
wstring WInfoFetcher::Model()
{
	wstringstream ss;

	vector<wstring> names = { L"Manufacturer", L"Model" };
	vector<CComVariant> values = { };

	HRESULT hr = NOERROR;
	hr = this->QueryInstanceProperties(L"Win32_ComputerSystem", names, values);
	if (SUCCEEDED(hr))
	{
		this->FillStringValues(ss, values);
	}
	return ss.str();
}
wstring WInfoFetcher::Kernel()
{
	wstringstream ss;

	vector<wstring> names = { L"Version" };
	vector<CComVariant> values = { };

	HRESULT hr = NOERROR;
	hr = this->QueryInstanceProperties(L"Win32_OperatingSystem", names, values);
	if (SUCCEEDED(hr))
	{
		this->FillStringValues(ss, values);
	}
	return ss.str();
}
wstring WInfoFetcher::Uptime()
{
	wstringstream ss;

	vector<CComVariant> values;
	HRESULT hr = this->QueryInstanceProperties(
		L"Win32_OperatingSystem",
		vector<wstring>{L"LastBootUpTime", L"LocalDateTime"},
		values
	);
	if (FAILED(hr)) return ss.str();
	if (values.size() != 2) return ss.str();

	wistringstream s1(values[0].bstrVal);
	wistringstream s2(values[1].bstrVal);
	tm t1{};
	tm t2{};
	s1 >> get_time(&t1, L"%Y%m%d%H%M%S");
	s2 >> get_time(&t2, L"%Y%m%d%H%M%S");
	
	auto span=mktime(&t2) - mktime(&t1);

	int days = span / 3600 / 24;
	int hours = span % (3600 * 24) / 3600;
	int mins = span % (3600) / 60;


	bool empty = true;
	if (days > 0) 
	{
		ss << days;
		if (days > 1)
		{
			ss << " days";
		}
		else
		{
			ss << " day";
		}
		empty = false;
	}

	if (hours > 0)
	{
		if (!empty) ss << ", ";
		ss << hours;
		if (hours > 1)
		{
			ss << " hours";
		}
		else
		{
			ss << " hour";
		}
		empty = false;
	}

	if (!empty) ss << ", ";
	ss << mins;
	if (mins > 1)
	{
		ss << " mins";
	}
	else
	{
		ss << " min";
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
	SHELLEXECUTEINFO info = { 0 };
	info.cbSize = sizeof(info);
	info.lpFile = cmd;
	info.lpVerb = L"open";
	info.lpParameters = args;
	info.nShow = SW_HIDE;
	info.fMask = SEE_MASK_NOZONECHECKS | SEE_MASK_NOASYNC | SEE_MASK_NOCLOSEPROCESS;
	ShellExecuteEx(&info);
	auto h = info.hProcess;

	DWORD code = 0;
	::GetExitCodeProcess(h, &code);
	

	

	return 0;
}


void WInfoFetcher::FillStringValues(wstringstream& ss, vector<CComVariant> const& values)
{
	for (auto&& v : values)
	{
		wstring text = v.bstrVal;
		if (!text.empty())
		{
			if (ss.tellp() != 0)
			{
				ss << " ";
			}
			ss << text.c_str();
		}
	}
}

