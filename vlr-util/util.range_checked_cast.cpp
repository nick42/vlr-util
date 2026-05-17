#include "pch.h"
#include "util.range_checked_cast.h"

#include "UtilMacros.Assertions.h"
#include "util.convert.StringConversion.h"

namespace vlr {

namespace util {

namespace detail {

void HandleOutOfRangeAssertionFailure(std::string_view svFailureMessage)
{
	auto sAugmentedFailureMessage = fmt::format(_T("{}{}"), VLR_ASSERTION_FAILURE_MESSAGE_PREFIX, vlr::util::Convert::ToFmtArg_String(svFailureMessage));
	vlr::assert::HandleCheckFailure(VLR_CODE_CONTEXT, sAugmentedFailureMessage);
}

} // namespace detail

} // namespace util

} // namespace vlr
