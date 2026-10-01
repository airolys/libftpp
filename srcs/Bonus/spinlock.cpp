#include "spinlock.hpp"

Spinlock::Spinlock()
	:	_flag(0)
{

}

void	Spinlock::lock()
{
	for (;;)
	{
		if (!_flag.exchange(true, std::memory_order_acquire))
			return ;

		while (_flag.load(std::memory_order_relaxed));
	}
}

void	Spinlock::unlock()
{
	_flag.store(false, std::memory_order_relaxed);
}

bool	Spinlock::try_lock()
{
	return (!_flag.load(std::memory_order_relaxed) &&
			!_flag.exchange(true, std::memory_order_acquire));
}
