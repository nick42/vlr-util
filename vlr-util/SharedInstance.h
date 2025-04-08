#pragma once

#include "config.h"

namespace vlr {

class ISharedInstanceBase
{
protected:
	virtual HRESULT OnCreate_DoInit() = 0;
	virtual HRESULT OnDestroy_DoCleanup() = 0;

	virtual void TrackingHook_OnCreate()
	{}
	virtual void TrackingHook_OnDestroy()
	{}

protected:
	HRESULT CallStateMethods_OnCreate()
	{
		TrackingHook_OnCreate();
		return OnCreate_DoInit();
	}
	HRESULT CallStateMethods_OnDestroy()
	{
		TrackingHook_OnDestroy();
		return OnDestroy_DoCleanup();
	}

	// TODO: Cleanup tracking?

protected:
	constexpr ISharedInstanceBase() = default;
	virtual ~ISharedInstanceBase() = default;
};

template< typename TSharedInstance >
class CSharedInstanceBase
	: public ISharedInstanceBase
{
public:
	const TSharedInstance& GetSharedInstance();
	TSharedInstance& GetSharedInstanceMutable();

protected:
	virtual HRESULT OnCreate_DoInit()
	{}
	virtual HRESULT OnDestroy_DoCleanup()
	{}

public:
	CSharedInstanceBase()
	{
		CallStateMethods_OnCreate();
	}
	virtual ~CSharedInstanceBase()
	{
		CallStateMethods_OnDestroy();
	}
};

} // namespace vlr
