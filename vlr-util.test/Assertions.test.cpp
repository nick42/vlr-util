#include "pch.h"

#include "vlr-util/zstring_view.h"

static std::function<void(vlr::tzstring_view svzFailureMessage)> g_fOnCheckFailure;

void MyHandleCheckFailure(vlr::tzstring_view svzFailureMessage)
{
	if (g_fOnCheckFailure)
	{
		g_fOnCheckFailure(svzFailureMessage);
	}
}

#include "vlr-util/UtilMacros.Assertions.h"
#include "vlr-util/ActionOnDestruction.h"
#include "vlr-util/util.std_aliases.h"

TEST(Assertions, HandleCheckFailure)
{
	vlr::assert::Callbacks::getSharedInstanceMutable().m_fHandleCheckFailure = &MyHandleCheckFailure;

	std::vector<vlr::tstring> vecAssertions;
	g_fOnCheckFailure = [&](vlr::tzstring_view svzFailureMessage) {
		vecAssertions.push_back(svzFailureMessage.toStdString());
	};
	auto oOnDestory_ClearCallback = vlr::MakeActionOnDestruction([&] { g_fOnCheckFailure = {}; });

	VLR_ASSERT_NONZERO_OR_CONTINUE(0);

	// Note: We expect this to write one assertion failure message
	EXPECT_EQ(vecAssertions.size(), 1);
}
