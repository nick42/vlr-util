#include "pch.h"
#include "ThreadOperationContext.h"

#include "logging.LogMessage.h"
#include "StringCompare.h"

namespace vlr {

namespace ThreadOperationContext {

SResult CContextLifetime::OnDestroy_PopContext()
{
	if (!m_spContext)
	{
		return SResult::Success_NoWorkDone;
	}

	return CThreadOperationContext::GetSharedInstance().PopContext(m_spContext);
}

SPCContextLifetime AddOperationContext(vlr::tzstring_view svzName, vlr::tzstring_view svzValue)
{
	static const auto _tFailureValue = SPCContextLifetime{};

	SResult sr;

	SPCGenericContext spContext;
	sr = CThreadOperationContext::GetSharedInstance().PushContext(svzName, svzValue, spContext);
	if (!sr.isSuccess())
	{
		return {};
	}
	VLR_ASSERT_NONZERO_OR_RETURN_FAILURE_VALUE(spContext);

	auto spContextLifetime = std::make_shared<CContextLifetime>(spContext);

	return spContextLifetime;
}

} // namespace ThreadOperationContext

SResult CThreadOperationContext::GetCurrentThreadContext(
	ThreadOperationContext::SPCPerThreadContext& spPerThreadContext_Result,
	bool bEnsureExistsInMap /*= false*/)
{
	auto oThreadID = std::this_thread::get_id();

	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	if (bEnsureExistsInMap)
	{
		auto& spPerThreadContext = m_mapThreadIDToContext[oThreadID];
		if (!spPerThreadContext)
		{
			spPerThreadContext = std::make_shared<ThreadOperationContext::CPerThreadContext>();
		}
		spPerThreadContext_Result = spPerThreadContext;

		return SResult::Success;
	}

	auto iterMapIndex = m_mapThreadIDToContext.find(oThreadID);
	if (iterMapIndex == m_mapThreadIDToContext.end())
	{
		return SResult::Success_NoWorkDone;
	}

	spPerThreadContext_Result = iterMapIndex->second;

	return SResult::Success;
}

SResult CThreadOperationContext::ClearCurrentThreadContext()
{
	auto oThreadID = std::this_thread::get_id();

	auto slDataAccess = std::scoped_lock{ m_mutexDataAccess };

	m_mapThreadIDToContext.erase(oThreadID);

	return SResult::Success;
}

SResult CThreadOperationContext::PopulateThreadOperationContext(std::vector<ThreadOperationContext::SPCGenericContext>& vecContextStack)
{
	ThreadOperationContext::SPCPerThreadContext spPerThreadContext;
	GetCurrentThreadContext(spPerThreadContext);
	if (!spPerThreadContext)
	{
		return SResult::Success_NoWorkDone;
	}
	auto& oPerThreadContext = *spPerThreadContext;

	for (auto iterListIndex = oPerThreadContext.m_listContextStack.begin(); iterListIndex != oPerThreadContext.m_listContextStack.end();)
	{
		auto wpContext = *iterListIndex;
		auto spContext = wpContext.lock();
		if (!spContext)
		{
			logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
				_T("Operation context invalid, but not properly removed; cleaning from context stack."));
			auto iterContextInvalid = iterListIndex++;
			oPerThreadContext.m_listContextStack.erase(iterContextInvalid);
			continue;
		}

		// Add context to result
		vecContextStack.push_back(spContext);

		++iterListIndex;
	}
	if (vecContextStack.empty())
	{
		// No more valid operation context for current thread; remove map entry
		ClearCurrentThreadContext();

		return SResult::Success_NoWorkDone;
	}

	return SResult::Success;
}

SResult CThreadOperationContext::PushContext(vlr::tzstring_view svzName, vlr::tzstring_view svzValue, ThreadOperationContext::SPCGenericContext& spContext_Result)
{
	// Note explicit allocation, because we use weak_ptr
	auto spContext = ThreadOperationContext::SPCGenericContext{ new ThreadOperationContext::CGenericContext(svzName, svzValue) };
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spContext);

	ThreadOperationContext::SPCPerThreadContext spPerThreadContext;
	GetCurrentThreadContext(spPerThreadContext, true);
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spPerThreadContext);

	spPerThreadContext->m_listContextStack.push_back(spContext);

	spContext_Result = spContext;

	return SResult::Success;
}

SResult CThreadOperationContext::PopContext(const ThreadOperationContext::SPCGenericContext& spContext)
{
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spContext);

	return PopContext(spContext->m_sName);
}

SResult CThreadOperationContext::PopContext(vlr::tzstring_view svzName)
{
	ThreadOperationContext::SPCPerThreadContext spPerThreadContext;
	GetCurrentThreadContext(spPerThreadContext);
	if (!spPerThreadContext)
	{
		logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
			_T("Attempted to remove operation context, but context stack was empty."));
		return SResult::Success_WithNuance;
	}
	if (spPerThreadContext->m_listContextStack.empty())
	{
		logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
			_T("Attempted to remove operation context, but context stack list was empty."));
		return SResult::Success_WithNuance;
	}

	while (!spPerThreadContext->m_listContextStack.empty())
	{
		auto wpContext = spPerThreadContext->m_listContextStack.back();
		auto spContext = wpContext.lock();
		if (!spContext)
		{
			logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
				_T("Operation context invalid, but not properly removed; cleaning from context stack."));
			spPerThreadContext->m_listContextStack.pop_back();
			continue;
		}

		if (StringCompare::CS().AreEqual(spContext->m_sName, svzName))
		{
			// This is the normal/expected case
			spPerThreadContext->m_listContextStack.pop_back();
			return SResult::Success;
		}

		// There is a valid context on the "top" of the stack, which is not what we're trying to pop
		// We will process this below.
		logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
			_T("Attempted to remove operation context, but not removing the most recent context."));
		break;
	}
	if (spPerThreadContext->m_listContextStack.empty())
	{
		logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
			_T("Attempted to remove operation context, but context stack list had no valid entries."));
		return SResult::Success_WithNuance;
	}

	for (auto iterListIndex = spPerThreadContext->m_listContextStack.rbegin(); iterListIndex != spPerThreadContext->m_listContextStack.rend();)
	{
		auto wpContext = *iterListIndex;
		auto spContext = wpContext.lock();
		if (!spContext)
		{
			logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
				_T("Operation context invalid, but not properly removed; cleaning from context stack."));
			auto iterContextInvalid = iterListIndex++;
			spPerThreadContext->m_listContextStack.erase(std::next(iterContextInvalid).base());
			continue;
		}

		if (StringCompare::CS().AreEqual(spContext->m_sName, svzName))
		{
			logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
				_T("Removing operation context: context removed from non-end of stack."));
			spPerThreadContext->m_listContextStack.erase(std::next(iterListIndex).base());
			return SResult::Success_WithNuance;
		}

		++iterListIndex;
	}

	logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
		_T("Attempted to remove operation context, but context was not found."));
	return SResult::Success_WithNuance;
}

} // namespace vlr
