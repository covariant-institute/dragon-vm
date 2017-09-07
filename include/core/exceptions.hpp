#pragma once

// DVM Exceptions
#include <exception>
#include <string>
#include <utility>

namespace dvm {
	namespace core {
		class exception final : public std::exception {
			std::string mWhat;
		public:
			exception() = delete;

			explicit exception(std::string what) : mWhat(std::move(what)) {}

			exception(const exception &) = default;

			exception(exception &&) = default;

			~exception() override = default;

			exception &operator=(const exception &)= default;

			exception &operator=(exception &&)= default;

			const char *what() const noexcept override
			{
				return this->mWhat.c_str();
			}
		};
	}
}
