#include "pch.h"
#include "vlr/ModuleContext.Runtime.CommandLine.h"

#include "vlr/ModuleContext.Compilation.h"

#ifdef _WIN32
#include <processenv.h>
#include <shellapi.h>
#endif

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( ModuleContext )

NAMESPACE_BEGIN( Runtime )

HRESULT CCommandLine::SetFromMainArgs( int argc, const char** argv )
{
	m_nArgCount = argc;
	m_ppArgArray_ASCII = argv;

	return S_OK;
}

HRESULT CCommandLine::SetFromMainArgs( int argc, const wchar_t** argv )
{
	m_nArgCount = argc;
	m_ppArgArray_UTF16 = argv;

	return S_OK;
}

HRESULT CCommandLine::SetFromAPI_Win32()
{
#ifndef _WIN32
	return S_FALSE;

#else
	auto pcszCommandLine = ::GetCommandLine();
	if constexpr (ModuleContext::Compilation::DefaultCharTypeIs_char())
	{
		//m_ppArgArray_ASCII = ::CommandLineToArgvA( pcszCommandLine, &m_nArgCount );
		// This does not exist on later versions of Windows
		return S_FALSE;
	}
	else
	{
		auto ppArgArray_UTF16 = ::CommandLineToArgvW( pcszCommandLine, &m_nArgCount );
		m_spAutoFreeArgArray = std::make_shared<vlr::CActionOnDestruction<void>>( vlr::MakeAutoCleanup_viaLocalFree( ppArgArray_UTF16 ) );
		m_ppArgArray_UTF16 = ppArgArray_UTF16;
	}

	return S_OK;

#endif
}

bool CCommandLine::HavePopulatedInfo() const
{
	return true
		&& (m_nArgCount > 0)
		&& (m_ppArgArray_ASCII || m_ppArgArray_UTF16)
		;
}

NAMESPACE_END //( Runtime )

NAMESPACE_END //( ModuleContext )

NAMESPACE_END //( vlr )
