#pragma once

#include "util.std_aliases.h"
#include "enums.FormatEnum.h"

NAMESPACE_BEGIN( vlr )

NAMESPACE_BEGIN( enums )

template<>
class CFormatEnum<SID_NAME_USE>
{
    using TEnum = SID_NAME_USE;
    using this_type = CFormatEnum<TEnum>;
    using base_type = CFormatEnumBase<DWORD_PTR>;

public:
    [[nodiscard]]
    static inline auto FormatValue( TEnum eValue )
        -> vlr::tstring
    {
        switch (eValue)
        {
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( SidTypeUser );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( SidTypeGroup );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( SidTypeDomain );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( SidTypeAlias );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( SidTypeWellKnownGroup );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( SidTypeDeletedAccount );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( SidTypeInvalid );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( SidTypeUnknown );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( SidTypeComputer );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( SidTypeLabel );
            VLR_ON_CASE_RETURN_STRING_OF_VALUE( SidTypeLogonSession );

        default:
            return base_type::FormatAsNumber( eValue );
        }
    }
};

NAMESPACE_END //( enums )

NAMESPACE_END //( vlr )
