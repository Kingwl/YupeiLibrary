#pragma once

#include "WindowBase.h"
#include "EventHandler.h"
#include "GraphicsBasis.h"
#include "SystemHelper.h"
#include "EventArgs.h"
#include "Control.h"

namespace Yupei
{

	extern LRESULT CALLBACK NormalWindowProc(
		HWND hwnd,
		UINT uMsg,
		WPARAM wParam,
		LPARAM lParam
		);

	struct WindowClassEx :public WNDCLASSEX
	{
		WindowClassEx();

		void Register() const
		{
			::RegisterClassEx(this);
		}
	};

	struct NativeWindowProperties : public IWindowProperties
	{
	public:
		DWORD ExStyle = 0;
		std::unique_ptr<WindowClassEx> WindowClass;
		UnicodeString WindowTitle;
		DWORD WindowStyle = WS_OVERLAPPEDWINDOW;
		int X = CW_USEDEFAULT;
		int Y = CW_USEDEFAULT;
		int Width = CW_USEDEFAULT;
		int Height = CW_USEDEFAULT;
		HWND ParentHandle = NULL;
		HMENU MenuHandle = NULL;
		HINSTANCE InstanceHandle = Windows::GetInstanceHandle();
		void* CustomParam = nullptr;

		NativeWindowProperties()
			:WindowClass(std::make_unique<WindowClassEx>())
		{

		}

		Graphics::Point GetWindowPos() const override
		{
			return Graphics::Point(X, Y);
		}

		Graphics::Size GetWindowSize() const override
		{
			return Graphics::Size(Width, Height);
		}

		UnicodeString GetWindowTitle() const override
		{
			return WindowTitle;
		}

		WindowBase* GetParentWindow() const override
		{
			return nullptr;
		}
	};

	class NativeWindow : public WindowBase,public ContentControl
	{
	public:
		friend LRESULT CALLBACK NormalWindowProc(
			HWND windowHandle,
			UINT msgID,
			WPARAM wParam,
			LPARAM lParam
			);

		EventHandler<WindowCreateArgs> WindowCreateEvent;
		EventHandler<WindowResizeArgs> WindowResizeEvent;
		EventHandler<WindowPaintArgs> WindowPaintEvent;
		
		//EventHandler OnSizing;

		enum /*class*/ WindowShownMethod
		{
			FirstShown = SW_SHOWNORMAL,
			NormalShown = SW_SHOW,
			ShownWithoutActivated = SW_SHOWNOACTIVATE,
			//ShownWithoutActivated = SW_SHOWNA,
			Hidden = SW_HIDE,
			Restored = SW_RESTORE,
			MaximizedWithActivated = SW_SHOWMAXIMIZED,
			MinimizedWithActivated = SW_SHOWMINIMIZED,
			MaximizedOnly = SW_MAXIMIZE,
			MinimizedOnly = SW_MINIMIZE
		};

		NativeWindow(RenderType type = RenderType::Hardware);
		explicit NativeWindow(NativeWindowProperties* properties, RenderType type = RenderType::Hardware);

		void Register(const WindowClassEx* windowClass) const;

		void Create(NativeWindowProperties* windowProperties);

		void Run() override;

		void RenderInternal(Graphics::IDrawingContext* dc) override
		{
			;
		}

		void NormalShow() override
		{
			::ShowWindow(windowHandle, WindowShownMethod::NormalShown);
		}

		void Hide() override
		{
			::ShowWindow(windowHandle, WindowShownMethod::Hidden);
		}

		void Destroy() override
		{
			::DestroyWindow(windowHandle);
		}

		void SetTiTle(const UnicodeString& title) override
		{
			::SetWindowText(windowHandle, title.data());
		}

		UnicodeString GetTitle() const override;

		Graphics::Size GetClientSize() const override
		{
			RECT rect;
			::GetClientRect(windowHandle, &rect);
			return Graphics::Size(rect.right - rect.left, rect.bottom - rect.top);
		}

		Graphics::Size GetWindowSize() const override
		{
			auto globalPosition = GetGlobalPosition();
			return Graphics::Size(globalPosition.right - globalPosition.left, globalPosition.bottom - globalPosition.top);
		}


		Graphics::Rect GetGlobalPosition() const override
		{
			RECT rect;
			::GetWindowRect(windowHandle, &rect);
			return Graphics::Rect(rect.left, rect.top, rect.right, rect.bottom);
		}

		void SetSize(const Graphics::Size& s) override
		{
			auto rect = GetGlobalPosition();
			::MoveWindow(windowHandle, rect.left, rect.top, rect.GetWidth(), rect.GetHeight(), TRUE);
		}

		void MoveTo(const Graphics::Point& point) override
		{
			auto windowSize = GetWindowSize();
			::MoveWindow(windowHandle, point.x, point.y, windowSize.width, windowSize.height, TRUE);
		}

		Graphics::IDrawingContext* GetContext() const override
		{
			return context;
		}

		void OnWindowResize(WPARAM wParam,LPARAM lParam)
		{
			resizeArgs->SetSize(lParam);
			SetActualWidth(resizeArgs->newSize.width);
			SetActualHeight(resizeArgs->newSize.height);
			resizeArgs->isHandled = false;
			if (renderType == RenderType::Hardware)
			{
				reinterpret_cast<Graphics::Direct2D::DrawingContext*>(context)->Resize(resizeArgs->newSize);
			}
			WindowResizeEvent.Invoke(this, resizeArgs);
			if(content)
				content->OnResize(false);
		}

		void OnRender(Graphics::IDrawingContext* _context) override
		{
			paintArgs->isHandled = false;
			_context->BeginDraw();
			_context->Clear(0);
			_context->FillRectangle(GetVirtualPosition(), backgroudBrush);
			if(content)
				content->OnRender(_context);
			WindowPaintEvent.Invoke(this, paintArgs);
			_context->EndDraw();
		}

		bool HitTest(const Graphics::Point& _point) const noexcept override
		{
			return true;
		}


		void InitializeRendering();
		~NativeWindow();

	private:
		MSG messages;
		Graphics::IDrawingContext* context;
		RenderType renderType;

		void InitializeArgs();
		//EventArgs
		
		WindowCreateArgs* createArgs = nullptr;
		WindowPaintArgs* paintArgs = nullptr;
		WindowNCPaintArgs* ncPaintArgs = nullptr;
		WindowResizeArgs* resizeArgs = nullptr;
	};




	
}