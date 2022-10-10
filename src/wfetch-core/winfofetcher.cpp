#include "pch.h"
#include "winfofetcher.h"

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
	auto&& s = _wbemServices.get();
	
	
	

	wstringstream ss;
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