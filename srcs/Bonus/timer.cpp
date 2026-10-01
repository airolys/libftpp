#include "timer.hpp"

void	Timer::set(std::time_t end)
{
	_end = end;
}

bool	Timer::check() const
{
	return (std::difftime(_end, std::time(nullptr)) >= 0);
}
