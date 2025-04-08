#pragma once

#include <optional>

#include "config.h"

#include "BaseWithVirtualDestructor.h"

namespace vlr {

template< typename TAssignedVariable, typename TAssignment = TAssignedVariable >
class CAutoRevertingAssignment
	: public CBaseWithVirtualDestructor
{
public:
	TAssignedVariable& m_tAssignedVariable;
	std::optional<TAssignment> m_otRevertToValue;

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
	// Note: Some assignment types cannot be stored in std::optional<> (eg: string literal as char array)
	// We will attempt to default to the assigned type if the assignment type cannot be stored
	constexpr bool bIsAssignmentTypeCompatible = true
		&& std::is_object_v<TAssignment>
		&& std::is_destructible_v<TAssignment>
		&& !std::is_array_v<TAssignment>;

	if constexpr (bIsAssignmentTypeCompatible)
	{
		return CAutoRevertingAssignment<TAssignedVariable, TAssignment>{ tAssignedVariable, tAssignment };
	}
	else
	{
		// Try to use the assigned variable as the storage type
		return CAutoRevertingAssignment<TAssignedVariable>{ tAssignedVariable, tAssignment };
	}
}

} // namespace vlr
