#include "persistent_worker.hpp"

PersistentWorker::PersistentWorker()
	:	_stop(false), _worker(std::thread([this]()
	{
		std::unique_lock<std::mutex>	lock(this->_mutex);
		while (true)
		{
			_cv.wait(lock, [&]{
				return (this->_stop || !this->_tasks.empty());
			});

			if (_stop)
				break;
			std::vector<std::string> snapshot;
			snapshot.reserve(this->_tasks.size());
			for (const auto& [name, _] : _tasks)
			{
				snapshot.push_back(name);
			}
			lock.unlock();
			for (const std::string &name : snapshot)
			{
				std::function<void()>	task;
				{
					std::lock_guard<std::mutex>	lock(_mutex);
					std::unordered_map<std::string,std::function<void()>>::iterator	it = this->_tasks.find(name);
					if (it == this->_tasks.end())
						continue;
					task = it->second;
				}
				task();
			}
			lock.lock();
		}
	}))
{

}

PersistentWorker::~PersistentWorker()
{
	stop();
	_worker.join();
}

void	PersistentWorker::addTask(const std::string& name, const std::function<void()>& task)
{
	{
		std::lock_guard<std::mutex>	lock(_mutex);
		_tasks[name] = task;
	}
	_cv.notify_one();
}

void	PersistentWorker::removeTask(const std::string& name)
{
	{
		std::lock_guard<std::mutex>	lock(_mutex);
		_tasks.erase(name);
	}
	_cv.notify_one();
}

void	PersistentWorker::start()
{
	{
		std::lock_guard<std::mutex>	lock(_mutex);
		_stop = false;
	}
	_cv.notify_one();
}

void	PersistentWorker::stop()
{	
	{
		std::lock_guard<std::mutex>	lock(_mutex);
		_stop = true;
	}
	_cv.notify_one();
}