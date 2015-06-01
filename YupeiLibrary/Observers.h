#pragma once

#include "Common.h"

namespace Yupei
{
	class IObserver;
	class Subject;
	class IObserver
	{
	public:
		virtual void Update(Subject* subject) = 0;
	};
	class Subject
	{
	public:
		void Attach(IObserver* observer)
		{
			observers.push_back(observer);
		}
		void Detach(IObserver* observer)
		{
			if (observers.empty()) return;
			auto pos = find(observers.begin(), observers.end(), observer);
			if(pos != observers.end()) observers.erase(pos);
		}
		void Notify()
		{
			for (auto& o : observers)
			{
				o->Update(this);
			}
		}
	private:
		vector<IObserver*> observers;
	};

	
}