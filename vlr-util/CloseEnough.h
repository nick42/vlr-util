#pragma once

#include <map>
#include <memory>
#include <numeric>
#include <string>
#include <vector>

#include "config.h"

#include "StringCompare.h"
#include "util.Result.h"
#include "UtilMacros.Assertions.h"

namespace vlr {

namespace CloseEnough {

// This is declared early, because it will be passed through methods
struct CandidateEval;

static constexpr double dWeightDefault_BasicCharacterAdd = 0.8;
static constexpr double dWeightDefault_BasicCharacterRemove = 0.8;
static constexpr double dWeightDefault_BasicCharacterChange = 1.0;

class CTransformTypeBase
{
public:
	// Given a current state, get the probable weight if a transform of this type was done
	virtual SResult GetProbableWeightForTransform(
		const CandidateEval& oCandidateEval,
		double& dWeight_Result) = 0;
	virtual double GetProbableWeightForTransformInline(
		const CandidateEval& oCandidateEval)
	{
		double dWeight = {};
		auto sr = GetProbableWeightForTransform(oCandidateEval, dWeight);
		if (!sr.isSuccess())
		{
			return {};
		}
		return dWeight;
	}
	// Evaluate for a transform of this type, and if possible, perform one
	// Success indicates that the transform type pushed a new transform to the list
	virtual SResult EvalForAddTransformStep(
		CandidateEval& oCandidateEval) = 0;
};
using SPCTransformTypeBase = std::shared_ptr<CTransformTypeBase>;

class CTransformType_BasicCharacterAdd
	: public CTransformTypeBase
{
public:
	virtual SResult GetProbableWeightForTransform(
		const CandidateEval& /*oCandidateEval*/,
		double& dWeight_Result)
	{
		dWeight_Result = dWeightDefault_BasicCharacterAdd;
		return SResult::Success;
	}
	virtual SResult EvalForAddTransformStep(
		CandidateEval& oCandidateEval);

};

class CTransformType_BasicCharacterRemove
	: public CTransformTypeBase
{
public:
	virtual SResult GetProbableWeightForTransform(
		const CandidateEval& /*oCandidateEval*/,
		double& dWeight_Result)
	{
		dWeight_Result = dWeightDefault_BasicCharacterRemove;
		return SResult::Success;
	}
	virtual SResult EvalForAddTransformStep(
		CandidateEval& oCandidateEval);

};

class CTransformType_BasicCharacterChange
	: public CTransformTypeBase
{
public:
	virtual SResult GetProbableWeightForTransform(
		const CandidateEval& /*oCandidateEval*/,
		double& dWeight_Result)
	{
		dWeight_Result = dWeightDefault_BasicCharacterChange;
		return SResult::Success;
	}
	virtual SResult EvalForAddTransformStep(
		CandidateEval& oCandidateEval);

};

class CTransformStepResultBase
{
public:
	// Store before/after?
	vlr::tstring m_sBefore;
	vlr::tstring m_sAfter;
	double m_dTransformCost = {};
};
using SPCTransformStepResultBase = std::shared_ptr<CTransformStepResultBase>;

class CSyntheticTransformStepResult_LevenshteinDistance
	: public CTransformStepResultBase
{
};

struct CandidateEval
{
	vlr::tstring m_sStartingValue;
	vlr::tstring m_sEndingValue;
	std::vector<SPCTransformStepResultBase> m_oTransformStepResultCollection;

	inline vlr::tstring GetMostRecentCandidateTransformState() const
	{
		const auto& _tFailureValue = m_sStartingValue;

		if (m_oTransformStepResultCollection.empty())
		{
			return m_sStartingValue;
		}
		const auto& spTransformStepResult = m_oTransformStepResultCollection.back();
		VLR_ASSERT_NONZERO_OR_RETURN_FAILURE_VALUE(spTransformStepResult);

		return spTransformStepResult->m_sAfter;
	}
	inline bool IsTransformComplete() const
	{
		const auto& oStringCompare = StringCompare::CS();
		return oStringCompare.AreEqual(GetMostRecentCandidateTransformState(), m_sEndingValue);
	}
	inline auto GetTotalTransformCost() const
	{
		static const auto fAddStepToTotalCost = [](double dSum, const SPCTransformStepResultBase& spTransformStep)
		{
			return dSum + spTransformStep->m_dTransformCost;
		};
		return std::accumulate(m_oTransformStepResultCollection.begin(), m_oTransformStepResultCollection.end(), 0.0, fAddStepToTotalCost);
	}
};
using SPCandidateEval = std::shared_ptr<CandidateEval>;

class CCloseEnough
{
public:
	std::vector<SPCTransformTypeBase> m_oTransformTypeCollection;
	std::map<double, SPCTransformTypeBase> m_mapProbableWeightToTransformType;

public:
	// Note: Order is not important here; will be sorted when put in map
	static const auto& GetDefaultTransformTypeCollection()
	{
		static auto oTransformTypeCollection_Default = []()
		{
			std::vector<SPCTransformTypeBase> oTransformTypeCollection;
			oTransformTypeCollection.push_back(std::make_shared<CTransformType_BasicCharacterAdd>());
			oTransformTypeCollection.push_back(std::make_shared<CTransformType_BasicCharacterRemove>());
			oTransformTypeCollection.push_back(std::make_shared<CTransformType_BasicCharacterChange>());

			return oTransformTypeCollection;
		}();
		return oTransformTypeCollection_Default;
	}

public:
	SResult EvaluateCandidates(
		const vlr::tstring& sStartingValue,
		const std::vector<vlr::tstring>& oTargetCollection,
		std::vector<CloseEnough::SPCandidateEval>& oCandidateEvalCollection_Result) const;
	SResult DoCandidateEval(CloseEnough::SPCandidateEval spCandidateEval) const;
	SResult DoCandidateEval_viaLevenshteinDistance(CloseEnough::SPCandidateEval spCandidateEval) const;

protected:
	SResult onConstructor();
public:
	CCloseEnough()
	{
		onConstructor();
	}
};

} // namespace CloseEnough

using CloseEnough::CCloseEnough;

} // namespace vlr
