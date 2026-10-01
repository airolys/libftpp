#pragma once

#include <vector>
#include <memory>
#include <functional>
#include <condition_variable>
//#include "thread_safe_queue.hpp"
#include <queue>

class WorkerPool
{

	class IJob
	{
	public:

		virtual void	execute() = 0;	//will be called to execute the job
		virtual ~IJob() = default;
	};

	class Job : public WorkerPool::IJob
	{
	private:

		std::function<void()>	_function;

	public:

		Job(const std::function<void()> &function)
			: _function(function)
		{
		};

		virtual void execute() override
		{
			_function();
		};
	};

private:

	std::mutex				_mutex;
	std::condition_variable	_cv;
	bool					_stop;
	std::queue<std::unique_ptr<IJob> >		_jobs;

	std::vector<std::thread>	_workers;

public:

	WorkerPool(int size);
	~WorkerPool();

	void	addJob(const std::function<void()> &jobToExecute);

};
