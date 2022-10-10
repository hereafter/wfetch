#include "pch.h"
#include "winfofetcher.h"
#include <atlbase.h>

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
	vector<wstring> values = { };

	HRESULT hr = NOERROR;
	hr = this->QueryInstanceProperties(L"Win32_OperatingSystem", names, values);
	if (SUCCEEDED(hr))
	{
		ss << values[0].c_str() << " " << values[1].c_str();
	}

	
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
	vector<wstring>& values)
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
				values.push_back(vt.bstrVal);
			}
		}
		vt.Clear();	
	}
	return hr;
}