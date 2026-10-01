#include "message.hpp"

Message::Message(int type)
	:	_type(type)
{

}

Message::Message(const Message& m)
	:	_type(m._type), _buff(m._buff)
{

}

int	Message::type() const
{
	return (_type);
}

DataBuffer&	Message::buff()
{
	return (_buff);
}

const std::vector<char>&	Message::getRaw() const
{
	return _buff.getRaw();
}

void	Message::setRaw(const std::vector<char>& in)
{
	_buff.setRaw(in);
}