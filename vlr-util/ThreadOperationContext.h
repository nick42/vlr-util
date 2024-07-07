#pragma once

#include <chrono>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "util.std_aliases.h"
#include "util.Result.h"
#include "zstring_view.h"

namespace vlr {

namespace ThreadOperationContext {

class CGenericContext
{
public:
	vlr::tstring m_sName;
	vlr::tstring m_sValue;

	virtual const vlr::tstring& GetName() const
	{
		return m_sName;
	}
	virtual const vlr::tstring& GetValue() const
	{
		return m_sValue;
	}

public:
	CGenericContext() = default;
	CGenericContext(vlr::tzstring_view svzName, vlr::tzstring_view svzValue)
		: m_sName{ svzName.toStdString() }
		, m_sValue{ svzValue.toStdString() }
	{}
};
using SPCGenericContext = std::shared_ptr<CGenericContext>;
using WPCGenericContext = std::weak_ptr<CGenericContext>;

class CActiveContext
{
public:
	vlr::tstring m_sKey;
	std::vector<WPCGenericContext> m_vecContextStack;

};

class CPerThreadContext
{
public:
	std::vector<CActiveContext> m_vecActiveContext;

protected:
	SResult FindActiveContext(
		vlr::tzstring_view svzName,
		CActiveContext*& pContextStack,
		bool bEnsureExists = false);

public:
	SResult PushContext(const SPCGenericContext& spContext);
	SResult PopContext(const SPCGenericContext& spContext);
	SResult PopContext(vlr::tzstring_view svzName, const SPCGenericContext& spContextToPop = {});

	SResult PopulateThreadOperationContext(std::vector<ThreadOperationContext::SPCGenericContext>& vecContextStack);

};
using SPCPerThreadContext = std::shared_ptr<CPerThreadContext>;

class CContextLifetime
{
public:
	SPCGenericContext m_spContext;

protected:
	SResult OnDestroy_PopContext();
public:
	~CContextLifetime()
	{
		OnDestroy_PopContext();
	}
	constexpr CContextLifetime() = default;
	CContextLifetime(const SPCGenericContext& spContext)
		: m_spContext{ spContext }
	{}
};
using SPCContextLifetime = std::shared_ptr<CContextLifetime>;

// Adds an operation context to the stack for the current thread, returning the context lifetime ptr.
// When the context lifetime goes out of scope, the destructor will pop the context automatically.
SPCContextLifetime AddOperationContext(vlr::tzstring_view svzName, vlr::tzstring_view svzValue);

} // namespace ThreadOperationContext

class CThreadOperationContext
{
public:
	static auto& GetSharedInstance()
	{
		static auto theInstance = CThreadOperationContext{};
		return theInstance;
	}

protected:
	std::mutex m_mutexDataAccess;
	std::unordered_map<std::thread::id, ThreadOperationContext::SPCPerThreadContext> m_mapThreadIDToContext;

	SResult GetCurrentThreadContext(
		ThreadOperationContext::SPCPerThreadContext& spPerThreadContext_Result,
		bool bEnsureExistsInMap = false);
	SResult ClearCurrentThreadContext();

public:
	SResult PushContext(vlr::tzstring_view svzName, vlr::tzstring_view svzValue, ThreadOperationContext::SPCGenericContext& spContext_Result);
	SResult PopContext(const ThreadOperationContext::SPCGenericContext& spContext);
	SResult PopContext(vlr::tzstring_view svzName, const ThreadOperationContext::SPCGenericContext& spContextToPop = {});

	// Returns S_FALSE if there is no context for the current thread
	SResult PopulateThreadOperationContext(std::vector<ThreadOperationContext::SPCGenericContext>& vecContextStack);

};

} // namespace vlr
