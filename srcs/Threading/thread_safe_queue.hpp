#pragma once

#include <deque>
#include <mutex>
#include <stdexcept>

template <typename TType>
class ThreadSafeQueue
{
	private:

		std::deque<TType>	_dequeue;
		std::mutex			_mutex;
		
	public:

		void	push_back(const TType&	value);
		void	push_front(const TType&	value);
		TType	pop_back(void);
		TType	pop_front(void);

		bool	empty()
		{
			const std::lock_guard<std::mutex>	lock(_mutex);
			return (_dequeue.empty());
		}

};

template <typename TType>
void	ThreadSafeQueue<TType>::push_back(const TType& value)
{
	const std::lock_guard<std::mutex>	lock(_mutex);

	_dequeue.push_back(value);
}

template <typename TType>
void	ThreadSafeQueue<TType>::push_front(const TType& value)
{
	const std::lock_guard<std::mutex>	lock(_mutex);

	_dequeue.push_front(value);
}

template <typename TType>
TType	ThreadSafeQueue<TType>::pop_back(void)
{
	const std::lock_guard<std::mutex>	lock(_mutex);

	if (_dequeue.empty())
		throw	std::runtime_error("Queue is empty");
	TType	value = std::move(_dequeue.back());
	_dequeue.pop_back();
	return (value);
}

template <typename TType>
TType	ThreadSafeQueue<TType>::pop_front(void)
{
	const std::lock_guard<std::mutex>	lock(_mutex);

	if (_dequeue.empty())
		throw	std::runtime_error("Queue is empty");
	TType	value = std::move(_dequeue.front());
	_dequeue.pop_front();
	return (value);
}
