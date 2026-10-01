#ifndef MEMENTO_HPP
# define MEMENTO_HPP

#include "../DataStructures/data_buffer.hpp"

class Memento
{

	public:

		using Snapshot = DataBuffer;
	
		Snapshot	save();

		void		load(const Memento::Snapshot& state);
	
	private:
		//pure virtual methods that needs to be inherited and implemented by other classes
		virtual void	_saveToSnapshot(Memento::Snapshot& snapshot) const = 0;
		virtual void	_loadFromSnapshot(Memento::Snapshot& snapshot) = 0;
};

#endif