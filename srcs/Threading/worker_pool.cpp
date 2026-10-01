#include "worker_pool.hpp"

WorkerPool::WorkerPool(int size)
	: _stop(false)
{
	_workers.reserve(size);
	for (int i=0; i<size; i++)
	{
		_workers.emplace_back([this]()
		{
			while (true)
			{
				std::unique_ptr<IJob> exe;
				{
					std::unique_lock<std::mutex> lock(this->_mutex);
					this->_cv.wait(lock, [this]
						{
							return (!this->_jobs.empty() || this->_stop);
						});
				
					if (_stop && _jobs.empty())
						break;
					exe = std::move(_jobs.front());
					_jobs.pop();
				}//guard scope end
				exe->execute();
			}
		});
	}
}

WorkerPool::~WorkerPool()
{
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_stop = true;
	}
	_cv.notify_all();
	for (std::thread& i : _workers)
	{
		if (i.joinable())
			i.join();
	}
}

void	WorkerPool::addJob(const std::function<void()> &jobToExecute)
{
	{
		std::lock_guard<std::mutex>	lock(_mutex);
		//_jobs.emplace(jobToExecute);
		_jobs.push(std::make_unique<Job>(std::move(jobToExecute)));
	}
	_cv.notify_one();
}
