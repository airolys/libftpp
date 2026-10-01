#pragma once

template <typename TType>
TType *Singleton<TType>::instance()
{
	if (!_instance)
		throw std::runtime_error("Instance not yet created");
	return (_instance);
}

template <typename TType>
template <typename... TArgs>
void Singleton<TType>::instantiate(TArgs&&... p_args)
{
	if (_instance)
		throw std::runtime_error("Instance already created");
	_instance = new TType(p_args ...);
}
