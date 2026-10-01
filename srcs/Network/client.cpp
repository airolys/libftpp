#include "client.hpp"

Client::Client()
	:	_socket(::socket(AF_INET, SOCK_STREAM, 0)),
		_epoll(epoll_create1(0)),
		conn(_socket)
{

}

void	Client::connect(const std::string& address, const size_t& port)
{
	if (_epoll < 0)
		throw std::runtime_error("epoll_create1 failed!");

	sockaddr_in	_distant;
	if (::inet_pton(AF_INET, address.c_str(), &_distant.sin_addr) <= 0)
		throw std::runtime_error("invalid address");
	_distant.sin_family = AF_INET;
	_distant.sin_port = htons(port);

	if (_socket < 0)
		throw std::runtime_error("socket failed");

	if (::connect(_socket, (sockaddr*)&_distant, sizeof(_distant)) < 0)
	{
		perror("connect");
		throw std::runtime_error("connect failed");
	}

	int flags = fcntl(_socket, F_GETFL, 0);
	fcntl(_socket, F_SETFL, flags | O_NONBLOCK);

	epoll_event	ev;
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = _socket;

	if (epoll_ctl(_epoll, EPOLL_CTL_ADD, _socket, &ev) < 0)
		throw std::runtime_error("epoll ctl add failed!");
}

void	Client::disconnect()
{
	if (_socket > 0)
		close(_socket);
	_socket = -1;

}

void	Client::defineAction(int messageType, const std::function<void(const Message& msg)>& action)
{
	_actions[messageType] = action;

	conn.onMessage([this](const Message& m) {
		auto it = _actions.find(m.type());
		if (it == _actions.end())
			throw std::runtime_error("No handler for message type");
		it->second(m);
	});
}

void	Client::send(const Message& msg)
{
	conn.sendMsg(msg);
}

void	Client::update()
{
	epoll_event	events[4];
	int n = epoll_wait(_epoll, events, 4, -1);//return immediatly
	if (n < 0)
		throw std::runtime_error("Epoll wait error");
	for (int i=0; i<n; i++)
	{
		uint32_t event = events[i].events;
		if (event & (EPOLLERR | EPOLLHUP))
			continue;
		if (event & EPOLLIN)
		{
			if (conn.onReadable() <= 0)
			{
				std::cout << "disconnected" << std::endl;
				exit(1);
			}
		}
		if (event & EPOLLOUT)
		{
			conn.onWritable();
		}
	}
}