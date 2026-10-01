#include "memento.hpp"

Memento::Snapshot	Memento::save()
{
	Snapshot	state;
	_saveToSnapshot(state);
	return (state);
}

void		Memento::load(const Memento::Snapshot& state)
{
	Snapshot	temp(state);
	_loadFromSnapshot(temp);
}

