#pragma once

#include "Common.h"

namespace Yupei
{
	template<typename ArgType>
	struct EventHandler
	{
	public:
		using EventFunction = Function<void(Object* , ArgType*)>;
		vector<EventFunction>& operator +=(const EventFunction& func)
		{
			events.push_back(func);
			return events;
		}

		void Invoke(Object* sender, ArgType* eventArgs)
		{
				for (auto&& func : events)
				{
					if (!eventArgs->isHandled)
						func(sender,eventArgs);
					else break;
				}
		}

	private:
		vector<EventFunction> events;
	};
}