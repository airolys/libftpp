#include "thread.hpp"

Thread::Thread(const std::string &name, std::function<void()> funct) :
	_status(INITIAL),
	_name(name),
	_thread(std::thread([this, funct]()
	{
		threadSafeCout.setPrefix(this->_name);

		std::unique_lock<std::mutex> lock(this->_mutex);
		_cv.wait(lock, [this] { return (this->_status != INITIAL); });

		
		if (this->_status == STARTED)
		{
			lock.unlock();
			funct();
			lock.lock();
			this->_status = FINISHED;
		}
		lock.unlock();
	}))
{

}

Thread::~Thread()
{
	stop();
}

void Thread::start()
{
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_status == INITIAL)
			_status = STARTED;
	}
	_cv.notify_one();
}

void	Thread::stop()
{
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (_status == INITIAL)
			_status = ABORTED;
		else if (_status == STARTED)
			_status = STOPPED;
	}
	_cv.notify_one();
	if (_thread.joinable())
		_thread.join();
}