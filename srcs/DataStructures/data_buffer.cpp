#include "data_buffer.hpp"

DataBuffer::DataBuffer(const DataBuffer& db)
{
	if (this != &db)
		*this = db;
}

DataBuffer&	DataBuffer::operator=(const DataBuffer& db)
{
	if (this != &db)
		_buffer = db._buffer;
	return (*this);
}

const std::vector<char>&	DataBuffer::getRaw() const
{
	return _buffer;
}

void	DataBuffer::setRaw(const std::vector<char>& in)
{
	_buffer = in;
}