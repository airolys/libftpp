#pragma once

#include <map>
#include "connection.hpp"
#include <sys/epoll.h>
#include <fcntl.h>
#include <unistd.h>

#define EPOLL_MAX 1024

class Server
{
	private:

	int		_listenfd;
	int		_epoll;

	std::unordered_map<int, Connection>	_connections;
	std::unordered_map<int, std::function<void(const long long&, const Message&)>> _actions;
	
	public:

	Server();

	void	start(const size_t& port);
	void	defineAction(int messageType, const std::function<void(const long long &clientID, const Message& msg)>& action);

	void	sendTo(const Message& msg, long long clientID);
	void	sendToArray(const Message& msg, std::vector<long long>& clientIDs);
	void	sendToAll(const Message& msg);

	void	update();
};
