#pragma once

#include <thread>
#include <condition_variable>
#include <mutex>
#include <unordered_map>
#include <functional>
#include <string>
#include <vector>

class PersistentWorker
{

private:

	std::mutex	_mutex;
	std::condition_variable	_cv;
	bool				_stop;
	std::unordered_map<std::string, std::function<void()>> _tasks;
	std::thread	_worker;
		
public:

	PersistentWorker();
	~PersistentWorker();

	void	addTask(const std::string& name, const std::function<void()>& task);
	void	removeTask(const std::string& name);

	void	start();
	void	stop();
};