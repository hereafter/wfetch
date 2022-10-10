#pragma once

using namespace winrt;

class WQuerySink: public implements<WQuerySink, IWbemObjectSink>
{
public:
	WQuerySink();

public: //IWebemObjectSink
	STDMETHODIMP Indicate(long lObjectCount,IWbemClassObject** apObjArray);
	STDMETHODIMP SetStatus(LONG lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject __RPC_FAR* pObjParam);

};

