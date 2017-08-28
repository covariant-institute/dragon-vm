#pragma once
// DVM Exceptions
#include <exception>
#include <string>
namespace dvm::core {
    class exception final:public std::exception {
		std::string mWhat;
	public:
		exception()=delete;
        exception(const std::string& what):mWhat(what) {}
        exception(const exception&)=default;
		exception(exception&&)=default;
		virtual ~exception()=default;
		exception& operator=(const exception&)=default;
		exception& operator=(exception&&)=default;
		virtual const char* what() const noexcept override
		{
			return this->mWhat.c_str();
		}
	};
}
