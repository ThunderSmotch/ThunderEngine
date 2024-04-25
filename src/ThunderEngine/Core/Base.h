#pragma once

//#include <memory>

#define TE_DERIVED(func, ...) static_cast<Derived*>(this)->func(__VA_ARGS__)