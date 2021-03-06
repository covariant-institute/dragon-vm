#pragma once

#include <core/type.hpp>

namespace dvm {
	namespace core {
		namespace config {
			using VersionID = Int32;

			/**
			 * 用来标识合法的 DVM 相关文件
			 * 文件的第一个字节等于 DVM_MAGIC 时，VM 才认为这是合法的可执行文件
			 * @see make_version_id()
			 */
			constexpr UInt8 DVM_MAGIC = 0xdc;

			/**
			 * 遵循 major.minor.patch 版本规则
			 */
			constexpr UInt8 VERSION_MAJOR = 0;
			constexpr UInt8 VERSION_MINOR = 1;
			constexpr UInt8 VERSION_PATCH = 0;

			/**
			 * 默认的 VM 栈大小 (10 K)
			 */
			constexpr UInt32 STACK_DEFAULT_SIZE = 1024 * 10;

			/**
			 * 默认的 VM 堆大小 (1 M)
			 */
			constexpr UInt32 HEAP_DEFAULT_SIZE = 1024 * 1024;

			/**
			 * 用当前的 VM 信息组合出一个 VersionID
			 * @return VersionID
			 */
			constexpr VersionID make_version_id()
			{
				return (DVM_MAGIC << 24)
				       | (VERSION_MAJOR << 16)
				       | (VERSION_MINOR << 8)
				       | (VERSION_PATCH);
			}

			/**
			 * Check if the version_id is compatible with current VM.
			 * @param version_id Target VersionID
			 * @return true if is valid
			 */
			bool validate_version_id(VersionID version_id);
		}
	}
}
