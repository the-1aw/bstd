/*
** EPITECH PROJECT, 2018
** cpp1
** File description:
** UnixLibrary
*/

#include "Library.hpp"

#pragma once

#include <dlfcn.h>

namespace bstd {
	class UnixLibrary : public bstd::Library {
	public:
		UnixLibrary() = delete;
		UnixLibrary(const std::string &libName) : Library(libName), _handle(dlopen(libName.c_str(), RTLD_LAZY)) {
			if (nullptr == _handle) {
				throw std::runtime_error(dlerror());
			}
		};

		~UnixLibrary() {
			if (nullptr != _handle)
				dlclose(_handle);
		}

	public:
		void *loadSymbol(const std::string &name) noexcept override {
			return (dlsym(this->_handle, name.c_str()));
		}

		private:
			void *_handle;
	};
}