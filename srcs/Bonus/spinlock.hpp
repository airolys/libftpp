#pragma once

#include <atomic>
#include <time.h>


//	https://rigtorp.se/spinlock/

class Spinlock
{
	private:

	std::atomic<unsigned int>	_flag;

	public:

	Spinlock();

	void	lock();
	void	unlock();

	bool	try_lock();
};
