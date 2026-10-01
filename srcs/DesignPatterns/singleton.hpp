#pragma once

#include <stdexcept>

template <typename TType>
class Singleton
{
	private:

		static TType*	_instance;

		Singleton();
	
		Singleton(const Singleton& s) = delete;
		Singleton&	operator=(const Singleton& s) = delete;

	public:

		static TType*	instance();

		template <typename ... TArgs>
		static void	instantiate(TArgs&&... p_args);
};

template <typename TType>
TType* Singleton<TType>::_instance = nullptr;	//global instance

#include "singleton.tpp"
