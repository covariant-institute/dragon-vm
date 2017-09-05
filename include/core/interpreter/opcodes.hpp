#pragma once

#include "core/type.hpp"

namespace dvm {
	namespace core {
		namespace interpreter {
			/**
			 * 为了方便我们使用 Threaded-Interpretation，指令一律用 UInt32
			 */
			enum class vm_opcode : UInt32 {
				VM_OP_NOP = 0,

                VM_OPCODES_NUMBER,
			};
		}
	}
}
