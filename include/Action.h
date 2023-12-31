#pragma once

#include <functional>

template <typename... Ts>
class Action
{
	std::vector<std::function<void(Ts...)>> callbacks {};

public:
	Action() = default;
	Action(std::function<void(Ts...)> func)
	{
		callbacks.emplace_back(func);
	}

	void operator+=(const std::function<void(Ts...)>& func)
	{
		callbacks.emplace_back(func);
	}
	void operator-=(const std::function<void(Ts...)>& func)
	{
		std::erase(callbacks, func);
	}

	void operator+=(const Action& action)
	{
		callbacks.emplace_back([&action] { action(); });
	}

	void operator()(Ts... args) const
	{
		for (auto& func : callbacks)
			func(args...);
	}
};
