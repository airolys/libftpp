#pragma once

#include "data_buffer.hpp"

/*
	Trivial types
*/

template <TrivialSeriazable T>
DataBuffer& DataBuffer::operator<<(const T &other)
{
	const std::size_t	size = sizeof(T);
	const char*	data = reinterpret_cast<const char *>(&other);

	_buffer.insert(_buffer.end(), data, data + size);
	return (*this);
}

template <TrivialSeriazable T>
DataBuffer& DataBuffer::operator>>(T &other)
{
	const std::size_t	size = sizeof(T);
	
	if (_buffer.size() < size)
		throw std::runtime_error("Cannot deserialize: not enough data in buffer.");

	std::memcpy(&other, _buffer.data(), size);
	_buffer.erase(_buffer.begin(), _buffer.begin() + size);
	return (*this);
}

/*
	Sequenced containers
*/

template <SequencedContainer T>
DataBuffer&	DataBuffer::operator<<(const T& other)
{
	uint32_t size = static_cast<uint32_t>(other.size());

	// Convert to network byte order
	uint32_t net_size = htonl(size);
	*this << net_size;

	for (const typename T::value_type val : other)
		*this << val;
	return (*this);
}

template <SequencedContainer T>
DataBuffer&	DataBuffer::operator>>(T& other)
{
	uint32_t net_size;
	*this >> net_size;

	// Convert to host byte order
	uint32_t size = ntohl(net_size);

	other.clear();
	for (uint32_t i=0; i<size; i++)
	{
		typename T::value_type val;
		*this >> val;
		other.push_back(val);
	}
	return (*this);
}

/*
	Associative containers
*/

template <AssociativeContainer T>
DataBuffer&	DataBuffer::operator<<(const T& other)
{
	uint32_t size = static_cast<uint32_t>(other.size());

	// Convert to network byte order
	uint32_t net_size = htonl(size);
	*this << net_size;

	for (typename T::const_iterator it = other.begin(); it != other.end(); it++)
	{
		const typename T::key_type		key = it->first;
		const typename T::mapped_type	val = it->second;
		*this << key << val;
	}
	return (*this);
}

template <AssociativeContainer T>
DataBuffer&	DataBuffer::operator>>(T& other) {
	uint32_t net_size;
	*this >> net_size;

	// Convert to host byte order
	uint32_t size = ntohl(net_size);

	other.clear();
	for (uint32_t i=0; i<size; i++)
	{
		typename T::key_type		key;
		typename T::mapped_type		val;

		*this >> key >> val;
		other.insert(std::make_pair(key, val));
	}
	return (*this);
}
