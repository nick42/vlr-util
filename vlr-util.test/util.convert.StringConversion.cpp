#include "pch.h"

#include "vlr/util.convert.StringConversion.h"

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

	};

	fTestConvertToAllTargetTypes( straTest );
	fTestConvertToAllTargetTypes( strwTest );
	fTestConvertToAllTargetTypes( svaTest );
	fTestConvertToAllTargetTypes( svwTest );
	fTestConvertToAllTargetTypes( saTest );
	fTestConvertToAllTargetTypes( swTest );
}
