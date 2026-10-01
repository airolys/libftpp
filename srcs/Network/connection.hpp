#pragma once

#include <vector>
#include <functional>
#include "message.hpp"

#define BUFF_SIZE 1024

class Connection
{
	private:

		int			_socket;

		uint32_t	_len;
		uint32_t	_type;
		std::vector<char>	_readBuff;
		std::vector<char>	_writeBuff;

		std::function<void(const Message&)> _onMessage;//template?

	public:

		Connection(int socket, const std::function<void(const Message&)>& action);
		Connection(int socket);
		Connection();

		void	sendMsg(const Message& m);
		void	onMessage(const std::function<void(const Message&)> action);
		int		onReadable();
		void	onWritable();

};
