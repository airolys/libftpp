#pragma once

#include "../DataStructures/data_buffer.hpp"


class Message
{
	private:
	
	const int	_type;
	DataBuffer	_buff;

	public:

	Message(int type);
	Message(const Message& m);

	int	type() const;
	DataBuffer&	buff();

	template <typename T>
	Message&	operator<<(const T& other)
	{
		_buff << other;
		return (*this);
	}

	template <typename T>
	const Message&	operator>>(T& other) const
	{
		DataBuffer	tmp(_buff);
		tmp >> other;
		return (*this);
	}

	const std::vector<char>&	getRaw() const;
	void	setRaw(const std::vector<char>& in);
};