#include "thread_safe_iostream.hpp"

std::mutex ThreadSafeiostream::_lock;

thread_local std::string ThreadSafeiostream::_prefix;
thread_local std::string ThreadSafeiostream::_buffer;

ThreadSafeiostream threadSafeCout;
//	templated class implementation in the header