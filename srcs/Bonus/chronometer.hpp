#pragma once

#include <ctime>

class Chronometer
{
	private:

	std::time_t	_begin;

	public:

	void	set();

	std::time_t	check();
};
