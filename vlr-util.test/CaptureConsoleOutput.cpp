#include "pch.h"

#include <iostream>
#include <catch2/catch_all.hpp>

#include "vlr-util/CaptureConsoleOut.h"
#include "vlr-util/util.Result.h"

using vlr::StandardResult;

static constexpr auto svzCheckValue_StdOut = "Some text";
static constexpr auto svzCheckValue_StdErr = "Some error";
static constexpr auto svzCheckValue_Missing = "Didn't print this...";

TEST_CASE("CaptureConsoleOut: general")
{
	StandardResult sr;

	auto oCaptureConsoleOut = vlr::util::CCaptureConsoleOut{};

	sr = oCaptureConsoleOut.BeginCapture();
	REQUIRE(sr == StandardResult::Success);

	std::cout << svzCheckValue_StdOut << std::endl;
	std::cerr << svzCheckValue_StdErr << std::endl;

	sr = oCaptureConsoleOut.EndCapture();
	REQUIRE(sr == StandardResult::Success);

	{
		auto oDataAnalysisHelper_StdOut = oCaptureConsoleOut.GetDataAnalysisHelper_StdOut();

		auto nCheckValueIndex = oDataAnalysisHelper_StdOut.FindMatchingLineIndex_Contains(svzCheckValue_StdOut);
		CHECK(nCheckValueIndex == 0);

		nCheckValueIndex = oDataAnalysisHelper_StdOut.FindMatchingLineIndex_Contains(svzCheckValue_Missing);
		CHECK(nCheckValueIndex == -1);
	}

	{
		auto oDataAnalysisHelper_StdErr = oCaptureConsoleOut.GetDataAnalysisHelper_StdErr();

		auto nCheckValueIndex = oDataAnalysisHelper_StdErr.FindMatchingLineIndex_Contains(svzCheckValue_StdErr);
		CHECK(nCheckValueIndex == 0);

		nCheckValueIndex = oDataAnalysisHelper_StdErr.FindMatchingLineIndex_Contains(svzCheckValue_Missing);
		CHECK(nCheckValueIndex == -1);
	}
}

TEST_CASE("CaptureConsoleOut: CScopedConsoleDataCapture")
{
	StandardResult sr;

	auto oCaptureConsoleOut = vlr::util::CCaptureConsoleOut{};

	{
		auto oScopedConsoleCapture = oCaptureConsoleOut.GetScopedCapture();

		std::cout << svzCheckValue_StdOut << std::endl;
		std::cerr << svzCheckValue_StdErr << std::endl;
	}

	{
		auto oDataAnalysisHelper_StdOut = oCaptureConsoleOut.GetDataAnalysisHelper_StdOut();

		auto nCheckValueIndex = oDataAnalysisHelper_StdOut.FindMatchingLineIndex_Contains(svzCheckValue_StdOut);
		CHECK(nCheckValueIndex == 0);

		nCheckValueIndex = oDataAnalysisHelper_StdOut.FindMatchingLineIndex_Contains(svzCheckValue_Missing);
		CHECK(nCheckValueIndex == -1);
	}

	{
		auto oDataAnalysisHelper_StdErr = oCaptureConsoleOut.GetDataAnalysisHelper_StdErr();

		auto nCheckValueIndex = oDataAnalysisHelper_StdErr.FindMatchingLineIndex_Contains(svzCheckValue_StdErr);
		CHECK(nCheckValueIndex == 0);

		nCheckValueIndex = oDataAnalysisHelper_StdErr.FindMatchingLineIndex_Contains(svzCheckValue_Missing);
		CHECK(nCheckValueIndex == -1);
	}
}

TEST_CASE("CaptureConsoleOut: CaptureDataOverride")
{
	StandardResult sr;

	auto oCaptureConsoleOut = vlr::util::CCaptureConsoleOut{};

	std::vector<char> arrCaptureData;
	auto onDataCapture = [&](const char* pData, size_t size)
	{
		arrCaptureData.reserve(arrCaptureData.size() + size);
		for (size_t i = 0; i < size; ++i)
		{
			arrCaptureData.push_back(pData[i]);
		}
	};

	auto isValueInCapture = [&](auto svCheckValue)
	{
		auto svCaptureData = std::string_view{ arrCaptureData.data(), arrCaptureData.size() };
		return svCaptureData.find(svCheckValue) != std::string_view::npos;
	};

	SECTION("Capture both")
	{
		sr = oCaptureConsoleOut.SetCaptureDataOverride(onDataCapture);
		CHECK(sr == StandardResult::Success);

		{
			auto oScopedConsoleCapture = oCaptureConsoleOut.GetScopedCapture();

			std::cout << svzCheckValue_StdOut << std::endl;
			std::cerr << svzCheckValue_StdErr << std::endl;
		}

		CHECK(isValueInCapture(svzCheckValue_StdOut) == true);
		CHECK(isValueInCapture(svzCheckValue_StdErr) == true);
		CHECK(isValueInCapture(svzCheckValue_Missing) == false);
	}

	SECTION("Capture only stdout")
	{
		sr = oCaptureConsoleOut.SetCaptureDataOverride(onDataCapture, vlr::util::CCaptureConsoleOut::CaptureStdOut);
		CHECK(sr == StandardResult::Success);

		{
			auto oScopedConsoleCapture = oCaptureConsoleOut.GetScopedCapture(vlr::util::CCaptureConsoleOut::CaptureStdOut);

			std::cout << svzCheckValue_StdOut << std::endl;
			std::cerr << svzCheckValue_StdErr << std::endl;
		}

		CHECK(isValueInCapture(svzCheckValue_StdOut) == true);
		CHECK(isValueInCapture(svzCheckValue_StdErr) == false);
		CHECK(isValueInCapture(svzCheckValue_Missing) == false);
	}

	SECTION("Capture only stderr")
	{
		sr = oCaptureConsoleOut.SetCaptureDataOverride(onDataCapture, vlr::util::CCaptureConsoleOut::CaptureStdErr);
		CHECK(sr == StandardResult::Success);

		{
			auto oScopedConsoleCapture = oCaptureConsoleOut.GetScopedCapture(vlr::util::CCaptureConsoleOut::CaptureStdErr);

			std::cout << svzCheckValue_StdOut << std::endl;
			std::cerr << svzCheckValue_StdErr << std::endl;
		}

		CHECK(isValueInCapture(svzCheckValue_StdOut) == false);
		CHECK(isValueInCapture(svzCheckValue_StdErr) == true);
		CHECK(isValueInCapture(svzCheckValue_Missing) == false);
	}
}

TEST_CASE("CCaptureConsoleDataAnalysisHelper::DoesOutputContainContent")
{
	StandardResult sr;

	static constexpr auto svzSubsequentValue_StdOut = "Other text";

	auto oCaptureConsoleOut = vlr::util::CCaptureConsoleOut{};

	{
		auto oScopedConsoleCapture = oCaptureConsoleOut.GetScopedCapture();

		std::cout << svzCheckValue_StdOut << std::endl;
		std::cout << svzSubsequentValue_StdOut << std::endl;
	}

	{
		auto oDataAnalysisHelper_StdOut = oCaptureConsoleOut.GetDataAnalysisHelper_StdOut();

		bool bContainsValues = false;

		bContainsValues  = oDataAnalysisHelper_StdOut.DoesOutputContainContent(svzCheckValue_StdOut);
		CHECK(bContainsValues == true);

		bContainsValues = oDataAnalysisHelper_StdOut.DoesOutputContainContent(svzSubsequentValue_StdOut);
		CHECK(bContainsValues == true);

		auto arrExpectedSequence = std::vector<std::string>{ svzCheckValue_StdOut, svzSubsequentValue_StdOut };
		bContainsValues = oDataAnalysisHelper_StdOut.DoesOutputContainSequence(arrExpectedSequence);
		CHECK(bContainsValues == true);

		auto arrBadSequence_OutOfOrder = std::vector<std::string>{ svzSubsequentValue_StdOut, svzCheckValue_StdOut };
		bContainsValues = oDataAnalysisHelper_StdOut.DoesOutputContainSequence(arrBadSequence_OutOfOrder);
		CHECK(bContainsValues == false);

		auto arrBadSequence_OtherData = std::vector<std::string>{ svzCheckValue_StdOut, "Something else", svzSubsequentValue_StdOut};
		bContainsValues = oDataAnalysisHelper_StdOut.DoesOutputContainSequence(arrBadSequence_OtherData);
		CHECK(bContainsValues == false);
	}
}
