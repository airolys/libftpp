#ifndef OBSERVER_HPP
# define OBSERVER_HPP

#include <unordered_map>
#include <vector>
#include <functional>

template <typename TEvent>
class Observer
{
	private:

		std::unordered_map<TEvent, std::vector<std::function<void()> > >	_reach;

	public:

		void	subscribe(const TEvent& event, const std::function<void()>& lambda);

		void	notify(const TEvent& event);
};

#include "observer.tpp"

#endif
