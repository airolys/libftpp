#include "server.hpp"


Server::Server()
{
		
}

void	Server::start(const size_t& port)
{
	_listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (_listenfd < 0)
		throw std::runtime_error("socket failed!");
	int op = 1;
	if (setsockopt(_listenfd, SOL_SOCKET, SO_REUSEADDR, &op, sizeof(int)) < 0)
		throw std::runtime_error("setsockopt failed!");

	int flags = fcntl(_listenfd, F_GETFL, 0);
	fcntl(_listenfd, F_SETFL, flags | O_NONBLOCK);

	sockaddr_in	address;
	memset(&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);

	if (bind(_listenfd, reinterpret_cast<sockaddr*>(&address), sizeof(address)) < 0)
		throw std::runtime_error("bind failed!");

	if (listen(_listenfd, SOMAXCONN) < 0)
		throw std::runtime_error("listen failed!");

	epoll_event	ev;
	ev.events = EPOLLIN;
	ev.data.fd = _listenfd;

	_epoll = epoll_create1(0);
	if (_epoll < 0)
		throw std::runtime_error("epoll_create1 failed");

	if (epoll_ctl(_epoll, EPOLL_CTL_ADD, _listenfd, &ev) < 0)
		throw std::runtime_error("server start epoll ctl all failed!");
}

void	Server::defineAction(int messageType, const std::function<void(const long long &clientID, const Message& msg)>& action)
{
	_actions[messageType] = action;
	//update existing connections;
	for (std::unordered_map<int, Connection>::iterator it = _connections.begin(); it != _connections.end(); it++)
	{
		long long id = it->first;
		it->second.onMessage([this, id](const Message& m) {
			auto its = this->_actions.find(m.type());
			if (its == this->_actions.end())
				throw std::runtime_error("No handler for message type");
			its->second(id, m);
		});
	}
}

void	Server::sendTo(const Message& msg, long long clientID)
{
	_connections[clientID].sendMsg(msg);
}

void	Server::sendToArray(const Message& msg, std::vector<long long>& clientIDs)
{
	for (long long i : clientIDs)
	{
		_connections[i].sendMsg(msg);
	}
}

void	Server::sendToAll(const Message& msg)
{
	for (std::unordered_map<int, Connection>::iterator it = _connections.begin(); it != _connections.end(); it++)
	{
		it->second.sendMsg(msg);
	}
}

void	Server::update()
{
	epoll_event	events[EPOLL_MAX];

	int	n = epoll_wait(_epoll, events, EPOLL_MAX, 0);
	if (n < 0)
		throw std::runtime_error("epoll wait failed!");
	for (int i=0; i<n; i++)
	{
		uint32_t event = events[i].events;
		if (event & (EPOLLERR | EPOLLHUP))
			continue;
		int fd = events[i].data.fd;
		if (event & EPOLLIN)//	nonblocking read
		{
			if (fd == _listenfd)//can be while (0 < (newfd = accept(etc)))
			{
				int	newfd = accept(_listenfd, nullptr, nullptr);
				if (newfd < 0)
					throw std::runtime_error("accept failed!");

				int flags = O_NONBLOCK | fcntl(newfd, F_GETFL, 0);
				fcntl(newfd, F_SETFL, flags);

				epoll_event ev;
				ev.events = EPOLLIN | EPOLLOUT;
				ev.data.fd = newfd;
				if (epoll_ctl(_epoll, EPOLL_CTL_ADD, newfd, &ev) < 0)
					throw std::runtime_error("epoll_ctl ADD failed!");
				
				_connections[newfd] = Connection(newfd, [newfd, this](const Message& m){
					auto its = _actions.find(m.type());
					if (its == _actions.end())
						throw std::runtime_error("No handler for message type");
					its->second(newfd, m);
				});
				continue;
			}
			if (_connections[fd].onReadable() <= 0)
			{
				std::cout << fd << " disconnected" << std::endl;//to showcase disconnection
				_connections.erase(fd);
				close(fd);
				epoll_ctl(_epoll, EPOLL_CTL_DEL, fd, nullptr);
			}
		}
		if (event & EPOLLOUT)//	nonblocking write
		{
			_connections[fd].onWritable();
		}
	}
}
