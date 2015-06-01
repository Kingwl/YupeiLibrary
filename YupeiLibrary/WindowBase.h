#pragma once

#include "Common.h"
#include "SystemHelper.h"
#include "GraphicsBasis.h"

namespace Yupei
{
	//using namespace Graphics;
	class WindowBase;

	class IWindowProperties
	{
	public:
		virtual Graphics::Point GetWindowPos() const = 0;
		virtual Graphics::Size GetWindowSize() const = 0;
		virtual UnicodeString GetWindowTitle() const = 0;
		virtual WindowBase* GetParentWindow() const = 0;
	};

	class Graphics::IDrawingContext;
	class WindowBase
	{
	public:
		enum class RenderType
		{
			Hardware,Software
		};
		using NativeHandleType = HWND;
		static constexpr int PaintInterval = static_cast<int>(Yupei::Windows::TimeSpan::GetMillisFromSeconds(1.0 / 60.0));
		//virtual void Create(IWindowProperties*) = 0;
		virtual void Run() = 0;
		virtual void NormalShow() = 0;
		virtual void Hide() = 0;
		virtual void Destroy() = 0;
		virtual void SetTiTle(const UnicodeString&) = 0;
		virtual UnicodeString GetTitle() const = 0;
		virtual Graphics::Size GetClientSize() const = 0;
		virtual Graphics::Size GetWindowSize() const = 0;
		virtual Graphics::Rect GetGlobalPosition() const = 0;
		virtual void SetSize(const Graphics::Size& s) = 0;
		virtual void MoveTo(const Graphics::Point& p) = 0;
		virtual Graphics::IDrawingContext* GetContext() const = 0;
		NativeHandleType GetNativeHandle() const noexcept
		{
			return windowHandle;
		}
		virtual ~WindowBase();
	protected:
		HWND windowHandle;
	};

	struct WindowManager
	{
	public:
		static WindowManager* GetWindowManager()
		{
			if (!windowManager) windowManager = unique_ptr<WindowManager>(new WindowManager());
			return WindowManager::windowManager.get();
		}
		static void AddWindow(WindowBase* p)
		{
			GetWindowManager()->windowMap[p->GetNativeHandle()] = p;
		}
		static void AddWindow(WindowBase::NativeHandleType handle, WindowBase* p)
		{
			GetWindowManager()->windowMap[handle] = p;
		}
		static void RemoveWindow(WindowBase::NativeHandleType handle)
		{
			GetWindowManager()->windowMap.erase(handle);
		}
		static WindowBase* GetWindow(WindowBase::NativeHandleType handle)
		{
			auto it = GetWindowManager()->windowMap.find(handle);
			if (it != GetWindowManager()->windowMap.end())
				return it->second;
			return nullptr;
		}

	private:
		WindowManager() = default;
		map<HWND, WindowBase*> windowMap;
		static unique_ptr<WindowManager> windowManager;
	};

	inline WindowBase* GetWindow(WindowBase::NativeHandleType handle) noexcept
	{
		return WindowManager::GetWindow(handle);
	}
	inline void AddWindow(WindowBase* p) noexcept
	{
		WindowManager::AddWindow(p);
	}
	inline void AddWindow(WindowBase::NativeHandleType handle, WindowBase* p)
	{
		WindowManager::AddWindow(handle, p);
	}
	inline void RemoveWindow(WindowBase::NativeHandleType handle)
	{
		WindowManager::RemoveWindow(handle);
	}
}