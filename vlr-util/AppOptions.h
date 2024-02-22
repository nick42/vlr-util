#pragma once

namespace vlr {

class CAppOptions
{
public:
	static auto& GetSharedInstance()
	{
		static auto theInstance = CAppOptions{};
		return theInstance;
	}
};

} // namespace vlr
