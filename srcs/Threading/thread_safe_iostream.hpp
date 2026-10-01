#ifndef THREAD_SAFE_IOSTREAM_HPP
# define THREAD_SAFE_IOSTREAM_HPP

#include <iostream>
#include <sstream>
#include <mutex>

class ThreadSafeiostream
{
	private:

		static std::mutex						_lock;
		static thread_local std::string			_prefix;
		static thread_local std::string			_buffer;

	public:

		template<typename T>
		ThreadSafeiostream&	operator<<(const T& t)
		{
			std::ostringstream	oss;
			oss << t;

			_buffer += oss.str();
			return (*this);
		}

		ThreadSafeiostream&	operator<<(std::ostream& (*manip)(std::ostream&))
		{
			// Apply manipulator to a temporary stream to detect behavior
		if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
		{
			_buffer += '\n';
			flush();
		}
		else if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::flush))
		{
			flush();
		}
		else
		{
			// fallback: apply to temp stream
			std::ostringstream oss;
			manip(oss);
			_buffer += oss.str();
		}
		return *this;
		}


		void	setPrefix(const std::string& prefix)
		{
			_prefix = prefix;
		}

		template <typename T>
		void	prompt(const std::string& question, T& dest)
		{
			const std::lock_guard<std::mutex>	guard(_lock);

			std::cout << _prefix << question << std::endl;
			std::cin >> dest;
		}

		void	flush()
		{
			const std::lock_guard<std::mutex>	guard(_lock);

			std::cout << _prefix << _buffer;
			_buffer.clear();
		}
};

extern ThreadSafeiostream threadSafeCout;

#endif