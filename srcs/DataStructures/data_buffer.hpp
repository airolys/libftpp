#ifndef DATA_BUFFER_HPP
# define DATA_BUFFER_HPP

#include <vector>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <arpa/inet.h>

#include <type_traits>
#include <concepts> //?:D

#include <ranges>
#include <iterator>

//		Trivial types
template<typename T>
concept TrivialSeriazable = std::is_trivially_copyable_v<T>;

//		Tuple types
template <typename T>
concept TupleLike =
	requires() {
		typename std::tuple_size<T>::type;
	};

//		Sequence containers
template <typename T>
concept SequencedContainer =
	std::ranges::range<T>
	 && requires(T c, typename T::value_type v) {
		{ c.size() } -> std::convertible_to<std::size_t>;
		c.begin();
		c.end();
		c.push_back(v);
		c.clear();
	};

//		Associative containers
template <typename T>
concept AssociativeContainer =
	std::ranges::range<T>
	 && requires(T c, typename T::key_type k, typename T::mapped_type v) {
		{ c.size() } -> std::convertible_to<std::size_t>;
		c.begin();
		c.end();
		c.insert({k, v});
		c.clear();
	};

class DataBuffer
{
	private:

		std::vector<char>	_buffer;

	public:
	
		DataBuffer() = default;
		DataBuffer(const DataBuffer& DB);

		const std::vector<char>&	getRaw() const;
		void	setRaw(const std::vector<char>& in);

		DataBuffer&	operator=(const DataBuffer& db);

		template <TrivialSeriazable T>
		DataBuffer&	operator<<(const T &other);

		template <TrivialSeriazable T>
		DataBuffer&	operator>>(T &other);

		template <SequencedContainer T>
		DataBuffer&	operator<<(const T& other);

		template <SequencedContainer T>
		DataBuffer&	operator>>(T& other);

		template <AssociativeContainer T>
		DataBuffer&	operator<<(const T& other);

		template <AssociativeContainer T>
		DataBuffer&	operator>>(T& other);

};

#include "data_buffer.tpp"

#endif
