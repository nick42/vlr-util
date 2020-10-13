#pragma once

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "AutoFreeResource.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( ModuleContext )

NAMESPACE_BEGIN( Runtime )

class CCommandLine
{
public:
	LPCSTR m_pcaszCommandLine = nullptr;
	char const* const* m_ppArgArray_ASCII = nullptr;
	LPCWSTR m_pcwszCommandLine = nullptr;
	wchar_t const* const* m_ppArgArray_UTF16 = nullptr;

	int m_nArgCount = 0;
	//std::optional<std::vector<LPCSTR>> m_oArgArray_ASCII;
	//std::optional<std::vector<LPCWSTR>> m_oArgArray_UTF16;
protected:
	// If we parsed an arg array, free on destruction
	std::shared_ptr<vlr::CActionOnDestruction<void>> m_spAutoFreeArgArray;

public:
	HRESULT SetFromMainArgs( int argc, const char** argv );
	HRESULT SetFromMainArgs( int argc, const wchar_t** argv );
	HRESULT SetFromAPI_Win32();

	bool HavePopulatedInfo() const;

};

NAMESPACE_END //( Runtime )

NAMESPACE_END //( ModuleContext )

NAMESPACE_END //( vlr )
