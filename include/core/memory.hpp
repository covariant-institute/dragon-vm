#pragma once

#include <core/type.hpp>

namespace dvm {
	namespace core {
		struct mem_block_info final {
			type_identifier type_id;
			Addr data_size;
			Byte reachable;
		};

        void* dvm_malloc(SizeT size);
	}
}