#ifndef __INCLUDED_RUNTIME_EXCEPTION_H
#define __INCLUDED_RUNTIME_EXCEPTION_H

#include <stdexcept>

std::runtime_error RuntimeException(const char* fmt, ...);

#endif
