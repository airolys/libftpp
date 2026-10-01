#ifndef POOL_HPP
# define POOL_HPP  

#include <deque>
#include <stdexcept>
#include <memory>
#include <stack>

template <typename TType>
class Pool
{	
	private:
	
		using storage = std::aligned_storage_t<sizeof(TType), alignof(TType)>;

		std::deque<storage>	_memory;
		std::deque<TType *>						_available;

		void	release(TType* ptr) { _available.push_front(ptr); }
		
	public:

		class Object
		{
		private:
			TType*	_obj;
			Pool&	_pool;

		public:

			Object(TType* obj, Pool& pool)
				: _obj(obj), _pool(pool)
			{
			}

			Object(const Object&) = delete;
			Object& operator=(const Object&) = delete;

			TType*	operator->() { return (_obj); }
			
			~Object()
			{
				if (_obj)
				{
					_obj->~TType();
					_pool.release(_obj);
				}
			}
		};

		template <typename... TArgs>
		Pool<TType>::Object	acquire(TArgs... p_args);

		void	resize(const size_t& new_size);

};

#include "pool.tpp"

#endif