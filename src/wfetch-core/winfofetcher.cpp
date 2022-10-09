#include "pch.h"
#include "winfofetcher.h"

using namespace winrt;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::System;

wstring WInfoFetcher::Title()
{
	auto user=User::GetDefault();	
	auto name = user.GetPropertyAsync(KnownUserProperties::AccountName()).get();
	wstringstream ss;
	ss << unbox_value<hstring>(name).c_str();
	return ss.str();
}