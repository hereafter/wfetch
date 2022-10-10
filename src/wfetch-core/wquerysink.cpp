#include "pch.h"
#include "wquerysink.h"

WQuerySink::WQuerySink()
{}

HRESULT WQuerySink::Indicate(long lObjectCount, IWbemClassObject** apObjArray)
{
	return NOERROR;
}

HRESULT WQuerySink::SetStatus(LONG lFlags, HRESULT hResult, BSTR strParam, IWbemClassObject* pObjParam)
{
	return NOERROR;
}
