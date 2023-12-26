#pragma once

#include <optional>

#include "UtilMacros.Namespace.h"
#include "config.h"

#include "BaseWithVirtualDestructor.h"

namespace vlr {

template< typename TAssignedVariable >
class CAutoRevertingAssignment
	: public CBaseWithVirtualDestructor
{
public:
	TAssignedVariable& m_tAssignedVariable;
	std::optional<TAssignedVariable> m_otRevertToValue;

public:
	HRESULT ClearRevertToValue()
	{
		m_otRevertToValue = {};
		return S_OK;
	}
	HRESULT RevertAssignment()
	{
		if (!m_otRevertToValue.has_value())
		{
			return S_FALSE;
		}
		m_tAssignedVariable = m_otRevertToValue.value();
		return S_OK;
	}

public:
	CAutoRevertingAssignment( TAssignedVariable& tAssignedVariable, const TAssignedVariable& tAssignment )
		: m_tAssignedVariable{ tAssignedVariable }
		, m_otRevertToValue{ tAssignedVariable }
	{
		m_tAssignedVariable = tAssignment;
	}
	template< typename TAssignment, std::enable_if_t<std::is_convertible_v<TAssignment, TAssignedVariable>> >
	CAutoRevertingAssignment( TAssignedVariable& tAssignedVariable, const TAssignment& tAssignment )
		: m_tAssignedVariable{ tAssignedVariable }
		, m_otRevertToValue{ tAssignedVariable }
	{
		m_tAssignedVariable = tAssignment;
	}
	virtual ~CAutoRevertingAssignment()
	{
		RevertAssignment();
	}
};

template< typename TAssignedVariable, typename TAssignment >
inline auto MakeAutoRevertingAssignment( TAssignedVariable& tAssignedVariable, const TAssignment& tAssignment )
{
	return CAutoRevertingAssignment<TAssignedVariable>{ tAssignedVariable, tAssignment };
}

} // namespace vlr
