#include "connection.hpp"

Connection::Connection(int socket, const std::function<void(const Message&)>& action)
	:	_socket(socket), _onMessage(action)
{

}

Connection::Connection(int socket)
	:	_socket(socket),
		_len(0)
{

}

Connection::Connection()
	:	_socket(-1),
		_len(0)
{

}


void	Connection::sendMsg(const Message& m)
{
	std::vector<char> raw = m.getRaw();
	uint32_t len = htonl(raw.size());
	uint32_t type = htonl(m.type());
	//len
	_writeBuff.insert(_writeBuff.end(), (char*)&len, (char*)&len + 4);
	//type
	_writeBuff.insert(_writeBuff.end(), (char*)&type, (char*)&type + 4);
	//msg
	_writeBuff.insert(_writeBuff.end(), raw.begin(), raw.end());
}

void	Connection::onMessage(const std::function<void(const Message&)> action)
{
	_onMessage = std::move(action);
}

int Connection::onReadable()
{
	char	buff[BUFF_SIZE];
	ssize_t	n = recv(_socket, buff, BUFF_SIZE, 0);
	if (n < 0)
		throw std::runtime_error("recv error");
	if (n == 0)
		return (n);

	_readBuff.insert(_readBuff.end(), buff, buff + n);
	while (true)
	{
		if (_readBuff.size() < 4 + 4)//size of len + type (2*uint32) (pas super beau le numero magique)
			break;

		if (_len == 0)
		{
			memcpy(&_len, _readBuff.data(), 4);
			_len = ntohl(_len);
			memcpy(&_type, _readBuff.data()+4, 4);
			_type = ntohl(_type);
		}
		if (_readBuff.size() < 4 + 4 + _len)
			break;
		std::vector<char> msgRaw(_readBuff.begin() + 8, _readBuff.begin() + 8 + _len);
		_readBuff.erase(_readBuff.begin(), _readBuff.begin() + 8 + _len);
		std::cout << "msg len:" << _len << std::endl;
		Message m(_type);
		_len = 0;
		m.setRaw(msgRaw);
		if (_onMessage)
			_onMessage(m);
	}
	return (n);
}

void	Connection::onWritable()
{
	if (!_writeBuff.empty())
	{
		ssize_t	n = send(_socket, _writeBuff.data(), _writeBuff.size(), 0);
		if (n < 0)
			throw std::runtime_error("send failed!");
		_writeBuff.erase(_writeBuff.begin(), _writeBuff.begin() + n);
	}
}