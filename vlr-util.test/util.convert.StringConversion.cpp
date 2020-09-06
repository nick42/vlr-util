#include "pch.h"

#include "vlr/util.convert.StringConversion.h"
#include "vlr/zstring_view.h"

TEST( util_convert_StringConversion, general )
{
	using namespace vlr::util;

	constexpr auto pcaszTestString = "TestString";
	constexpr auto pcwszTestString = L"TestString";
	constexpr auto pcszTestString = _T("TestString");

	const auto straTest = std::string{ pcaszTestString };
	const auto strwTest = std::wstring{ pcwszTestString };
	const auto svaTest = std::string_view{ straTest };
	const auto svwTest = std::wstring_view{ strwTest };
	const auto svzaTest = vlr::zstring_view{ straTest };
	const auto svzwTest = vlr::wzstring_view{ strwTest };
	const auto saTest = CStringA{ pcaszTestString };
	const auto swTest = CStringW{ pcwszTestString };

	auto fTestConvertToAllTargetTypes = [&]( const auto& tString )
	{
		// to std::string

		auto tString_asStdStringA = Convert::ToStdStringA( tString );
		EXPECT_STREQ( tString_asStdStringA.c_str(), pcaszTestString );
		auto tString_asStdStringW = Convert::ToStdStringW( tString );
		EXPECT_STREQ( tString_asStdStringW.c_str(), pcwszTestString );
		auto tString_asStdStringT = Convert::ToStdString( tString );
		EXPECT_STREQ( tString_asStdStringT.c_str(), pcszTestString );

		// to CString

		auto tString_asCStringA = Convert::ToCStringA( tString );
		EXPECT_STREQ( tString_asCStringA, pcaszTestString );
		auto tString_asCStringW = Convert::ToCStringW( tString );
		EXPECT_STREQ( tString_asCStringW, pcwszTestString );
		auto tString_asCStringT = Convert::ToCString( tString );
		EXPECT_STREQ( tString_asCStringT, pcszTestString );
	};

	fTestConvertToAllTargetTypes( straTest );
	fTestConvertToAllTargetTypes( strwTest );
	fTestConvertToAllTargetTypes( svaTest );
	fTestConvertToAllTargetTypes( svwTest );
	fTestConvertToAllTargetTypes( svzaTest );
	fTestConvertToAllTargetTypes( svzwTest );
	fTestConvertToAllTargetTypes( saTest );
	fTestConvertToAllTargetTypes( swTest );
}
