#pragma once

//	Construct in place and hand out a managing Object, also resize up if empty
template <typename TType>
template <typename... TArgs>
typename Pool<TType>::Object	Pool<TType>::acquire(TArgs... p_args)
{
	if (_available.empty())
		resize(_memory.size() + 1);
			
	TType* ptr = _available.front();
	_available.pop_front();

	std::construct_at(ptr, std::forward<TArgs>(p_args)...);

	return (Object(ptr, *this));
}

//	Allocate space for an aligned TType (stored in a deque)
template <typename TType>
void	Pool<TType>::resize(const size_t& new_size)
{
	std::size_t	poolSize = _memory.size();

	if (new_size < poolSize)
		throw std::invalid_argument("Cannot resize pool below the number of active objects.");
	
	for (std::size_t i=poolSize; i<new_size; i++)
	{
		_memory.emplace_back();
		TType*	ptr = reinterpret_cast<TType*>(&_memory.back());
		_available.push_back(ptr);
	}
}
