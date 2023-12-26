#include "pch.h"
#include "CloseEnough.h"

#include <algorithm>
#include <functional>
#include <thread>

#include "util.LevenshteinDistance.h"

namespace vlr {

namespace CloseEnough {

SResult CTransformType_BasicCharacterAdd::EvalForAddTransformStep(
	CandidateEval& /*oCandidateEval*/)
{
	return SResult::Success_NoWorkDone;
}

SResult CTransformType_BasicCharacterRemove::EvalForAddTransformStep(
	CandidateEval& /*oCandidateEval*/)
{
	return SResult::Success_NoWorkDone;
}

SResult CTransformType_BasicCharacterChange::EvalForAddTransformStep(
	CandidateEval& /*oCandidateEval*/)
{
	return SResult::Success_NoWorkDone;
}

SResult CCloseEnough::EvaluateCandidates(
	const vlr::tstring& sStartingValue,
	const std::vector<vlr::tstring>& oTargetCollection,
	std::vector<SPCandidateEval>& oCandidateEvalCollection_Result) const
{
	std::vector<SPCandidateEval> oCandidateEvalCollection;
	oCandidateEvalCollection.reserve(oTargetCollection.size());
	for (const auto& sTarget : oTargetCollection)
	{
		auto spCandidateEval = std::make_shared<CandidateEval>();
		VLR_ASSERT_ALLOCATED_OR_RETURN_STANDARD_ERROR(spCandidateEval);
		spCandidateEval->m_sStartingValue = sStartingValue;
		spCandidateEval->m_sEndingValue = sTarget;
		oCandidateEvalCollection.push_back(spCandidateEval);
	}

	std::vector<std::thread> oEvalThreadCollection;
	for (const auto& spCandidateEval : oCandidateEvalCollection)
	{
		auto fDoCandidateEval = [this, spCandidateEval]
		{
			return CCloseEnough::DoCandidateEval_viaLevenshteinDistance(spCandidateEval);
		};
		oEvalThreadCollection.push_back(std::thread{ fDoCandidateEval });
	}

	for (auto& oThread : oEvalThreadCollection)
	{
		oThread.join();
	}

	// Populate results

	oCandidateEvalCollection_Result = oCandidateEvalCollection;
	auto fOrderResults = [](const SPCandidateEval& spLhs, const SPCandidateEval& spRhs)
	{
		return spLhs->GetTotalTransformCost() < spRhs->GetTotalTransformCost();
	};
	std::sort(oCandidateEvalCollection_Result.begin(), oCandidateEvalCollection_Result.end(), fOrderResults);

	return SResult::Success;
}

SResult CCloseEnough::DoCandidateEval(SPCandidateEval spCandidateEval) const
{
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spCandidateEval);

	SResult sr;
	for (const auto& oMapPair : m_mapProbableWeightToTransformType)
	{
		const auto& spTransformType = oMapPair.second;
		do
		{
			sr = spTransformType->EvalForAddTransformStep(*spCandidateEval);
			VLR_ASSERT_HR_SUCCEEDED_OR_RETURN_HRESULT(sr);
			if (sr == SResult::Success)
			{
				// We keep attempting to apply the same transform repeatedly until N/A
				continue;
			}
			// else we got S_FALSE, and we continue on to the next transform
			break;
		} while (true);
	}

	// By the end, we should have a fully transformed string
	VLR_ASSERT_COMPARE_OR_RETURN_EUNEXPECTED(spCandidateEval->GetMostRecentCandidateTransformState(), ==, spCandidateEval->m_sEndingValue);

	return SResult::Success;
}

SResult CCloseEnough::DoCandidateEval_viaLevenshteinDistance(SPCandidateEval spCandidateEval) const
{
	VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spCandidateEval);

	struct CustomDistanceCostEval
	{
		double getDeltaCost(TCHAR cSource, TCHAR cTarget)
		{
			if (std::toupper(cSource) == std::toupper(cTarget))
			{
				return 0;
			}
			// Note: Would like to increment between 0.5 and 1 per replacement, but this doesn't work,
			// because this is also called for non-taken paths. =/
			return 1.0;
		}
	};
	CustomDistanceCostEval oDistanceCostEval;

	auto dLogicalDistance = GeneralizedLevenshteinDistanceCustomCost<vlr::tstring, double, TCHAR>(
		spCandidateEval->m_sStartingValue,
		spCandidateEval->m_sEndingValue,
		dWeightDefault_BasicCharacterAdd,
		dWeightDefault_BasicCharacterRemove,
		[&](TCHAR cSource, TCHAR cTarget) { return oDistanceCostEval.getDeltaCost(cSource, cTarget); });

	auto spTransformResult = std::make_shared<CSyntheticTransformStepResult_LevenshteinDistance>();
	spTransformResult->m_sBefore = spCandidateEval->m_sStartingValue;
	spTransformResult->m_sAfter = spCandidateEval->m_sEndingValue;
	spTransformResult->m_dTransformCost = dLogicalDistance;

	spCandidateEval->m_oTransformStepResultCollection.push_back(spTransformResult);

	return SResult::Success;
}

SResult CCloseEnough::onConstructor()
{
	m_oTransformTypeCollection = GetDefaultTransformTypeCollection();
	for (const auto& spTransformType : m_oTransformTypeCollection)
	{
		VLR_ASSERT_NONZERO_OR_RETURN_EUNEXPECTED(spTransformType);
		auto dWeight = spTransformType->GetProbableWeightForTransformInline(CandidateEval{});
		m_mapProbableWeightToTransformType[dWeight] = spTransformType;
	}

	return SResult::Success;
}

} // namespace CloseEnough

} // namespace vlr
