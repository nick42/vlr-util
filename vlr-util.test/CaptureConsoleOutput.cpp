#include "pch.h"

#include <iostream>

#include "vlr-util/CaptureConsoleOut.h"
#include "vlr-util/util.Result.h"

using vlr::StandardResult;

TEST(CaptureConsoleOut, general)
{
	StandardResult sr;

	auto svzCheckValue_StdOut = "Some text";
	auto svzCheckValue_StdErr = "Some error";
	auto svzCheckValue_Missing = "Didn't print this...";

	auto oCaptureConsoleOut = vlr::util::CCaptureConsoleOut{};

	sr = oCaptureConsoleOut.BeginCapture();
	ASSERT_EQ(sr, StandardResult::Success);

	std::cout << svzCheckValue_StdOut << std::endl;
	std::cerr << svzCheckValue_StdErr << std::endl;

	sr = oCaptureConsoleOut.EndCapture();
	ASSERT_EQ(sr, StandardResult::Success);

	{
		auto oDataAnalysisHelper_StdOut = oCaptureConsoleOut.GetDataAnalysisHelper_StdOut();

		auto nCheckValueIndex = oDataAnalysisHelper_StdOut.FindMatchingLineIndex_Contains(svzCheckValue_StdOut);
		ASSERT_EQ(nCheckValueIndex, 0);

		nCheckValueIndex = oDataAnalysisHelper_StdOut.FindMatchingLineIndex_Contains(svzCheckValue_Missing);
		ASSERT_EQ(nCheckValueIndex, -1);
	}

	{
		auto oDataAnalysisHelper_StdErr = oCaptureConsoleOut.GetDataAnalysisHelper_StdErr();

		auto nCheckValueIndex = oDataAnalysisHelper_StdErr.FindMatchingLineIndex_Contains(svzCheckValue_StdErr);
		ASSERT_EQ(nCheckValueIndex, 0);

		nCheckValueIndex = oDataAnalysisHelper_StdErr.FindMatchingLineIndex_Contains(svzCheckValue_Missing);
		ASSERT_EQ(nCheckValueIndex, -1);
	}
}
