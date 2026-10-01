#pragma once

template <typename TEvent>
void Observer<TEvent>::subscribe(const TEvent &event, const std::function<void()>& lambda)
{
	_reach[event].push_back(lambda);
}

template <typename TEvent>
void Observer<TEvent>::notify(const TEvent &event)
{
	typename std::unordered_map<TEvent, std::vector<std::function<void()> > >::const_iterator	found = _reach.find(event);
	if (found != _reach.end())
	{
		for (const std::function<void()>& lambda : found->second)
			lambda();
	}
}
