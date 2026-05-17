#include "pch.h"

#include "vlr-util/CloseEnough.h"

TEST(CloseEnough, general)
{
	vlr::tstring sTestInput = _T("hisstur");

	std::vector<vlr::tstring> oCandidateValueCollection;
	oCandidateValueCollection.push_back(_T("config"));
	oCandidateValueCollection.push_back(_T("values"));
	oCandidateValueCollection.push_back(_T("history"));
	oCandidateValueCollection.push_back(_T("misc"));

	auto oCloseEnough = vlr::CCloseEnough{};

	std::vector<vlr::CloseEnough::SPCandidateEval> oCandidateEvalCollection;
	auto sr = oCloseEnough.EvaluateCandidates(sTestInput, oCandidateValueCollection, oCandidateEvalCollection);
	ASSERT_EQ(sr.isSuccess(), true);
	ASSERT_GT(oCandidateEvalCollection.size(), 0U);

	const auto& spFirstCandidateEval = oCandidateEvalCollection.front();
	ASSERT_NE(spFirstCandidateEval, nullptr);

	auto dTotalCost = spFirstCandidateEval->GetTotalTransformCost();
	ASSERT_EQ(spFirstCandidateEval->IsTransformComplete(), true);
	ASSERT_EQ(spFirstCandidateEval->m_sEndingValue, _T("history"));
}

TEST(CloseEnough, CaseInsensitive)
{
	vlr::tstring sTestInput = _T("Histur");

	std::vector<vlr::tstring> oCandidateValueCollection;
	oCandidateValueCollection.push_back(_T("config"));
	oCandidateValueCollection.push_back(_T("values"));
	oCandidateValueCollection.push_back(_T("history"));
	oCandidateValueCollection.push_back(_T("misc"));

	auto oCloseEnough = vlr::CCloseEnough{};

	std::vector<vlr::CloseEnough::SPCandidateEval> oCandidateEvalCollection;
	auto sr = oCloseEnough.EvaluateCandidates(sTestInput, oCandidateValueCollection, oCandidateEvalCollection);
	ASSERT_EQ(sr.isSuccess(), true);
	ASSERT_GT(oCandidateEvalCollection.size(), 0U);

	const auto& spFirstCandidateEval = oCandidateEvalCollection.front();
	ASSERT_NE(spFirstCandidateEval, nullptr);

	auto dTotalCost = spFirstCandidateEval->GetTotalTransformCost();
	ASSERT_EQ(spFirstCandidateEval->IsTransformComplete(), true);
	ASSERT_EQ(spFirstCandidateEval->m_sEndingValue, _T("history"));
}

TEST(CloseEnough, SpecificDelta_OneChar)
{
	vlr::tstring sTestInput = _T("histori");

	std::vector<vlr::tstring> oCandidateValueCollection;
	oCandidateValueCollection.push_back(_T("config"));
	oCandidateValueCollection.push_back(_T("values"));
	oCandidateValueCollection.push_back(_T("history"));
	oCandidateValueCollection.push_back(_T("misc"));

	auto oCloseEnough = vlr::CCloseEnough{};

	std::vector<vlr::CloseEnough::SPCandidateEval> oCandidateEvalCollection;
	auto sr = oCloseEnough.EvaluateCandidates(sTestInput, oCandidateValueCollection, oCandidateEvalCollection);
	ASSERT_EQ(sr.isSuccess(), true);
	ASSERT_GT(oCandidateEvalCollection.size(), 0U);

	const auto& spFirstCandidateEval = oCandidateEvalCollection.front();
	ASSERT_NE(spFirstCandidateEval, nullptr);

	auto dTotalCost = spFirstCandidateEval->GetTotalTransformCost();
	ASSERT_EQ(spFirstCandidateEval->IsTransformComplete(), true);
	ASSERT_EQ(spFirstCandidateEval->m_sEndingValue, _T("history"));
}
