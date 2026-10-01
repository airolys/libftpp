#pragma once

#include <string>
#include <thread>
#include <functional>
#include <condition_variable>

#include "thread_safe_iostream.hpp"

class Thread
{
	private:

		enum {INITIAL, STARTED, FINISHED, STOPPED, ABORTED} _status;
		std::mutex	_mutex;
		std::string	_name;
		std::condition_variable	_cv;
		std::thread	_thread;
	
	public:

		Thread(const std::string& name, std::function<void()> funct);
		~Thread();
		void	start();
		void	stop();
};