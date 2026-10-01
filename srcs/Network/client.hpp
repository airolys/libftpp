#pragma once

#include "connection.hpp"
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>


class Client
{
	private:

	int		_socket;
	int		_epoll;

	Connection	conn;
	std::unordered_map<int, std::function<void(const Message&)>> _actions;
	
	public:

	Client();

	void	connect(const std::string& address, const size_t& port);
	void	disconnect();
	void	defineAction(int messageType, const std::function<void(const Message& msg)>& action);
	void	send(const Message& msg);
	void	update();
};