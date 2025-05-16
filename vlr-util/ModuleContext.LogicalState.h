#pragma once

// Intent: This file should contain the logical state for the module, such that other code can
// trigger off this.
// Eg: If the component is in shutdown
// Note: This may not be updated by the app, and must be set externally, so it should not be 
// considered "reliable" except in cases where the consuming app manages it properly. Usage code 
// should always handle the Unknown cases appropriately.
// Note: This state may be invalid/suspect during shutdown.

#include "config.h"
#include "enums.SmartEnum.h"

namespace vlr {

namespace ModuleContext {

namespace LifecycleState {

enum ELifecycleState
{
	Unknown,
	PreMain,
	InMain,
	Shutdown,
	PostMain,
};
constexpr auto MAX_VALUE = PostMain;

class CFormatEnum
	: public vlr::enums::CFormatEnumBase<DWORD_PTR>
{
	using TEnum = ELifecycleState;
	using base_type = CFormatEnumBase<DWORD_PTR>;

public:
	[[nodiscard]]
	static inline auto FormatValue(ELifecycleState eValue)
		-> vlr::tstring
	{
		switch (eValue)
		{
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(Unknown);
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(PreMain);
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(InMain);
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(Shutdown);
			VLR_ON_CASE_RETURN_STRING_OF_VALUE(PostMain);

		default:
			return base_type::FormatAsNumber(eValue);
		}
	}
};

class CRangeCheckForEnum
	: public vlr::enums::CRangeInfoSequential_DWORD_PTR<MAX_VALUE>
{
	using TBaseEnum = ELifecycleState;
	using TParentType = CRangeInfoSequential_DWORD_PTR<MAX_VALUE>;

public:
	using TParentType::IsValueInRange;
	//static bool IsValueInRange(DWORD_PTR dwValue)
	//{
	//	return ThisType::IsValueInRange(DWORD_PTR dwValue);
	//}
	static TBaseEnum CheckedEnumCast(DWORD_PTR dwValue)
	{
		if (IsValueInRange(dwValue))
		{
			return (TBaseEnum)dwValue;
		}
		else
		{
			return (TBaseEnum)0;
		}
	}
};

using SELifecycleState = enums::SmartEnum<
	ELifecycleState,
	Unknown, 
	CFormatEnum, 
	CRangeCheckForEnum>;

} // namespace LifecycleState

class CLogicalState
{
public:
	static CLogicalState& GetSharedInstanceMutable();
	static const auto& GetSharedInstance()
	{
		return GetSharedInstanceMutable();
	}

public:
	LifecycleState::SELifecycleState m_eLifecycleState;

public:
	inline bool IsInNormalStateFor_ClassInit() const
	{
		switch (m_eLifecycleState)
		{
		case LifecycleState::Unknown:
		case LifecycleState::PreMain:
		case LifecycleState::InMain:
			return true;

			// If we're in a shutdown state, we should not be doing class init operations
		default:
			return false;
		}
	}

};

} // namespace ModuleContext

} // namespace vlr
