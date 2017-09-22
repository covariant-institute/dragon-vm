#pragma once

// DVM Exceptions
#include <exception>
#include <cstdio>
#include <string>
#include <utility>

namespace dvm {
    namespace core {
        template <typename...args_t>
        std::string sprintf(const std::string &format, args_t &&...args) {
            int size = ::snprintf(nullptr, 0, format.c_str(), std::forward<args_t>(args)...);
            if (size < 0) {
                size = 255;
            }

            ++size; // null terminated
            char buffer[size];
            ::snprintf(buffer, static_cast<size_t>(size),
                       format.c_str(),
                       std::forward<args_t>(args)...);
            return std::string(buffer);
        }

        class Exception : public std::exception {
        protected:
            std::string mWhat;
        public:
            Exception() = delete;

            template <typename...args_t>
            explicit Exception(const std::string &what, args_t &&...args) : mWhat(
                    dvm::core::sprintf(what, std::forward<args_t>(args)...)) { }

            Exception(const Exception &) = default;

            Exception(Exception &&) noexcept = default;

            virtual ~Exception() = default;

            Exception &operator=(const Exception &)= default;

            Exception &operator=(Exception &&)= default;

            virtual const char *what() const noexcept override {
                return this->mWhat.c_str();
            }
        };
    }
}
