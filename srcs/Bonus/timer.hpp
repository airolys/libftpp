#pragma once

#include <ctime>

class Timer
{
	private:

	std::time_t	_end;

	public:

	void	set(const std::time_t	end);

	bool	check() const;
};
