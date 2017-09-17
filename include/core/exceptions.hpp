#pragma once

// DVM Exceptions
#include <exception>
#include <cstdio>
#include <string>
#include <utility>

namespace dvm {
	namespace core {
		template<typename...args_t>std::string sprintf(const std::string& format,args_t&&...args) {
			static char buffer[2048];
			::snprintf(buffer,2048,format.c_str(),std::forward<args_t>(args)...);
			return std::string(buffer);
		}
		class exception : public std::exception {
		protected:
			std::string mWhat;
		public:
			exception() = delete;

			template<typename...args_t>explicit exception(const std::string& what,args_t&&...args) : mWhat(dvm::core::sprintf(what,std::forward<args_t>(args)...)) {}

			exception(const exception &) = default;

			exception(exception &&) noexcept = default;

			virtual ~exception() = default;

			exception &operator=(const exception &)= default;

			exception &operator=(exception &&)= default;

			virtual const char *what() const noexcept override
			{
				return this->mWhat.c_str();
			}
		};
	}
}
