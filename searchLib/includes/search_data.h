#pragma once
#include <search_info.h>
#include <mutex>

namespace search {
	
	struct SearchData {
	public:

		bool searchDone = false;
		uint64_t infoVersion;

		SearchInfo info;
	};
}