#pragma once

#include "Common.h"

namespace Yupei
{
	namespace Windows
	{
		//COM Helper
		template<typename InterfaceChain>
		struct ComBase : public InterfaceChain
		{
			HRESULT QueryInterface(REFIID iid, void** ppObject)
			{
				*ppObject = nullptr;
				InterfaceChain::QueryInterfaceInternal(iid, ppv);
				if (*ppObject == nullptr)
					return E_NOINTERFACE;
				AddRef();
				return S_OK;
			}

			void AddRef()
			{
				::InterlockedIncrement(&ReferenceCount);
			}

			void Release()
			{
				auto newCount = ::InterlockedDecrement(&ReferenceCount);
				if (newCount == 0) delete this;
				return newCount;
			}
			
			virtual ~ComBase() = default;
		private:
			ULONG ReferenceCount;
		};

		struct QiListNil
		{
		};

		template<typename TMe,typename InterfaceChain>
		struct QiListSelf : public InterfaceChain
		{
			void QueryInterfaceInternal(const IID& iid, void** ppObject)
			{
				*ppObject = nullptr;
				if (iid != __uuidof(TMe))
					return InterfaceChain::QueryInterfaceInternal(iid, ppObject);
				*ppObject = static_cast<TMe*>(this);
			}
		};

		template<typename InterfaceName>
		struct QiList:public InterfaceName
		{
			inline void QueryInterfaceInternal(IID const& iid, OUT void** ppObject) throw()
			{
				if (iid != __uuidof(InterfaceName))
					return;

				*ppObject = static_cast<InterfaceName*>(this);
			}
		};

		//TimeSpan

		struct TimeSpan
		{
			static constexpr int64_t TicksPerMillisecond = 10000;
			static constexpr int64_t TicksPerSecond = TicksPerMillisecond * 1000;
			static constexpr int64_t TicksPerMinute = TicksPerSecond * 60;
			static constexpr int64_t TicksPerHour = TicksPerMinute * 60;
			static constexpr int64_t TicksPerDay = TicksPerHour * 24;
			static constexpr int64_t MillisPerSecond = 1000;
			static constexpr int64_t MillisPerMinute = MillisPerSecond * 60;
			static constexpr int64_t MillisPerHour = MillisPerMinute * 60;
			static constexpr int64_t MillisPerDay = MillisPerHour * 24;

			TimeSpan(int64_t _ticks)
			{
				ticks = _ticks;
			}
		
			TimeSpan(int hours, int minutes, int seconds)
			{
				ticks = TimeToTicks(hours, minutes, seconds);
			}

			static TimeSpan FromMilliseconds(double value)
			{
				return Interval(value, 1);
			}

			static TimeSpan FromMinutes(double value)
			{
				return Interval(value, MillisPerMinute);
			}

			bool operator < (const TimeSpan& rhs) const noexcept
			{
				return ticks < rhs.ticks;
			}

			bool operator == (const TimeSpan& rhs) const noexcept
			{
				return ticks == rhs.ticks;
			}

			bool operator != (const TimeSpan& rhs) const noexcept
			{
				return ticks != rhs.ticks;
			}

			bool operator > (const TimeSpan& rhs) const noexcept
			{
				return ticks > rhs.ticks;
			}

			static constexpr double GetMillisFromSeconds(double seconds) noexcept
			{
				return seconds * MillisPerSecond;
			}

		private:
			int64_t ticks;

			static int64_t TimeToTicks(int hour, int minute, int second) noexcept
			{
				int64_t totalSeconds = (int64_t)hour * 3600 + (int64_t)minute * 60 + (int64_t)second;
				return totalSeconds * TicksPerSecond;
			}

			static TimeSpan Interval(double value, int scale)
			{
				double tmp = value * tmp;
				double millis = tmp + (value >= 0 ?  0.5 : -0.5);
				return TimeSpan(millis * TicksPerMillisecond);
			}


		};

		inline HINSTANCE GetInstanceHandle()
		{
			return ::GetModuleHandle(nullptr);
		}
	}
}