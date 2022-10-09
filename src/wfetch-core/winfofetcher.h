#pragma once


using namespace std;

class WInfoFetcher
{
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


private:
	wstring UserName();
	wstring HostName();
};
