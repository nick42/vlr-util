#include "pch.h"

#include <iostream>
#include <catch2/catch_all.hpp>

#include "vlr-util/CaptureConsoleOut.h"
#include "vlr-util/util.Result.h"

using vlr::StandardResult;

auto svzCheckValue_StdOut = "Some text";
auto svzCheckValue_StdErr = "Some error";
auto svzCheckValue_Missing = "Didn't print this...";

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
