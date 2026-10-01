#include "chronometer.hpp"

void	Chronometer::set()
{
	std::time(&_begin);
}

std::time_t	Chronometer::check()
{
	return (std::difftime(std::time(nullptr), _begin));
}
