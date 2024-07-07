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

SResult CPerThreadContext::FindActiveContext(
	vlr::tzstring_view svzName,
	CActiveContext*& pActiveContext,
	bool bEnsureExists /*= false*/)
{
	auto fCheckForMatchingContext = [&](const CActiveContext& oActiveContext)
	{
		return StringCompare::CS().AreEqual(oActiveContext.m_sKey, svzName);
	};
	auto iterIndex_ActiveContext = std::find_if(m_vecActiveContext.begin(), m_vecActiveContext.end(), fCheckForMatchingContext);

	if (iterIndex_ActiveContext != m_vecActiveContext.end())
	{
		pActiveContext = &(*iterIndex_ActiveContext);
		return SResult::Success;
	}

	// No existing entry

	if (!bEnsureExists)
	{
		return SResult::Success_WithNuance;
	}

	// Add new entry, and return iterator to it.

	auto& oActiveContext = m_vecActiveContext.emplace_back();
	oActiveContext.m_sKey = svzName.toStdString();

	pActiveContext = &oActiveContext;

	return SResult::Success;
}

SResult CPerThreadContext::PushContext(const SPCGenericContext& spContext)
{
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spContext);

	SResult sr;

	CActiveContext* pActiveContext{};
	sr = FindActiveContext(spContext->GetName(), pActiveContext, true);
	VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);
	VLR_ASSERT_COMPARE_OR_RETURN_EUNEXPECTED(pActiveContext, != , nullptr);

	auto& oActiveContext = *pActiveContext;

	oActiveContext.m_vecContextStack.push_back(spContext);

	return SResult::Success;
}

SResult CPerThreadContext::PopContext(const SPCGenericContext& spContext)
{
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spContext);

	return PopContext(spContext->GetName(), spContext);
}

SResult CPerThreadContext::PopContext(vlr::tzstring_view svzName, const SPCGenericContext& spContextToPop /*= {}*/)
{
	SResult sr;

	CActiveContext* pActiveContext{};
	sr = FindActiveContext(svzName, pActiveContext);
	VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);
	if (sr != SResult::Success)
	{
		logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
			_T("Attempted to remove operation context, but context stack for context key was empty."));
		return SResult::Success_WithNuance;
	}
	VLR_ASSERT_COMPARE_OR_RETURN_EUNEXPECTED(pActiveContext, != , nullptr);

	auto& oActiveContext = *pActiveContext;
	auto& vecContextStack = oActiveContext.m_vecContextStack;

	while (!vecContextStack.empty())
	{
		auto wpContext = vecContextStack.back();
		auto spContext = wpContext.lock();
		if (!spContext)
		{
			logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
				_T("Operation context invalid, but not properly removed; cleaning from context stack."));
			vecContextStack.pop_back();
			continue;
		}

		// All contexts in this stack should have the same key name
		if (!StringCompare::CS().AreEqual(spContext->GetName(), svzName))
		{
			VLR_ASSERTIONS_HANDLE_CHECK_FAILURE(_T("Unexpected context in stack"));
			vecContextStack.pop_back();
			continue;
		}

		if (spContextToPop)
		{
			if (!StringCompare::CS().AreEqual(spContext->GetValue(), spContextToPop->GetValue()))
			{
				logging::LogMessageFmt(VLR_LOG_CONTEXT_WARNING,
					_T("Attempted to remove operation context, but pop context value '{}' does not match top of stack value '{}'; ignoring pop."),
					spContextToPop->GetValue(),
					spContext->GetValue());
				break;
			}
		}

		// Normal case: matching context
		vecContextStack.pop_back();
		break;
	}
	if (vecContextStack.empty())
	{
		// Note: We will potentially remove from vector elsewhere
		return SResult::Success;
	}

	// Note: We do not go through the stack to remove other potentially invalid or duplicate entries now.
	// Duplicate entries are expected (context stack), and we can remove invalid entries as we encounter them in other processing.

	logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
		_T("Attempted to remove operation context, but context was not found on top of stack for key."));
	return SResult::Success_WithNuance;
}

SResult CPerThreadContext::PopulateThreadOperationContext(std::vector<ThreadOperationContext::SPCGenericContext>& vecContextStack)
{
	for (auto& oActiveContext : m_vecActiveContext)
	{
		auto& vecContextStack_Current = oActiveContext.m_vecContextStack;

		// Iterate in reverse order of the stack, to find the last valid context
		// Note: Under normal operations, this would be the last context entry, but we allow for the possibility that
		// items may be invalidated outside of a pop call.
		for (auto iterIndex = vecContextStack_Current.rbegin(); iterIndex != vecContextStack_Current.rend();)
		{
			auto wpContext = *iterIndex;
			auto spContext = wpContext.lock();
			if (!spContext)
			{
				logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
					_T("Operation context invalid, but not properly removed; cleaning from context stack."));
				auto iterContextInvalid = iterIndex++;
				vecContextStack_Current.erase(std::next(iterContextInvalid).base());
				continue;
			}

			// Add context to result
			vecContextStack.push_back(spContext);

			// We ignore any other "previous" values for this context item, as they are not active
			break;
		}
	}

	// Clean up any no longer active contexts

	auto fActiveContextIsEmpty = [&](const CActiveContext& oActiveContext)
	{
		return oActiveContext.m_vecContextStack.empty();
	};
	auto iterFirstToRemove = std::remove_if(m_vecActiveContext.begin(), m_vecActiveContext.end(), fActiveContextIsEmpty);
	m_vecActiveContext.erase(iterFirstToRemove, m_vecActiveContext.end());

	return SResult::Success;
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
	SResult sr;

	ThreadOperationContext::SPCPerThreadContext spPerThreadContext;
	GetCurrentThreadContext(spPerThreadContext);
	if (!spPerThreadContext)
	{
		return SResult::Success_NoWorkDone;
	}
	auto& oPerThreadContext = *spPerThreadContext;

	sr = spPerThreadContext->PopulateThreadOperationContext(vecContextStack);
	VLR_ASSERT_SR_SUCCEEDED_OR_RETURN_SRESULT(sr);

	// House keeping: If we had no context, we can clear the structure for the current thread

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
	SResult sr;

	// Note explicit allocation, because we use weak_ptr
	auto spContext = ThreadOperationContext::SPCGenericContext{ new ThreadOperationContext::CGenericContext(svzName, svzValue) };
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spContext);

	ThreadOperationContext::SPCPerThreadContext spPerThreadContext;
	GetCurrentThreadContext(spPerThreadContext, true);
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spPerThreadContext);

	sr = spPerThreadContext->PushContext(spContext);
	VLR_ON_SR_ERROR_RETURN_VALUE(sr);

	spContext_Result = spContext;

	return SResult::Success;
}

SResult CThreadOperationContext::PopContext(const ThreadOperationContext::SPCGenericContext& spContext)
{
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spContext);

	return PopContext(spContext->GetName(), spContext);
}

SResult CThreadOperationContext::PopContext(vlr::tzstring_view svzName, const ThreadOperationContext::SPCGenericContext& spContext /*= {}*/)
{
	ThreadOperationContext::SPCPerThreadContext spPerThreadContext;
	GetCurrentThreadContext(spPerThreadContext);
	if (!spPerThreadContext)
	{
		logging::LogMessageFmt(VLR_LOG_CONTEXT_VERBOSE,
			_T("Attempted to remove operation context, but context stack was empty."));
		return SResult::Success_WithNuance;
	}

	return spPerThreadContext->PopContext(svzName, spContext);
}

} // namespace vlr
