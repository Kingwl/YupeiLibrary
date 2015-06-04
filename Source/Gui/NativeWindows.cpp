#include "NativeWindows.h"
#include "EventArgs.h"
#include "Graphics.h"
#include "UIElement.h"

namespace Yupei
{
	LRESULT CALLBACK NormalWindowProc(
		HWND windowHandle,
		UINT msgID,
		WPARAM wParam,
		LPARAM lParam
		)
	{
		NativeWindow* mainWindow = static_cast<NativeWindow*>(GetWindow(windowHandle));
		bool isHandled = false;
		switch (msgID)
		{
		case WM_CREATE:
		{
			auto createStruct = reinterpret_cast<CREATESTRUCT*>(lParam);
			mainWindow = reinterpret_cast<NativeWindow*>(createStruct->lpCreateParams);
			AddWindow(windowHandle,mainWindow);
			auto args = mainWindow->createArgs;
			mainWindow->createArgs->createStructPtr = createStruct;
			mainWindow->WindowCreateEvent.Invoke(mainWindow,args);
			isHandled = args->isHandled;
			break;
		}
		case WM_PAINT:
		{
			mainWindow->OnRender(mainWindow->GetContext());
			isHandled = true;
		}
			break;
		case WM_DESTROY:
			::PostQuitMessage(0);
			break;
		case WM_ERASEBKGND:
			isHandled = true;
			break;
		case WM_SIZE:
		{
			mainWindow->OnWindowResize(wParam, lParam);
		}
			break;
		}
		if(!isHandled) return ::DefWindowProc(windowHandle, msgID, wParam, lParam);
		return 0;
	}

	WindowClassEx::WindowClassEx()
	{
		this->cbSize = sizeof(WNDCLASSEX);
		this->style = CS_HREDRAW | CS_VREDRAW;
		this->lpfnWndProc = NormalWindowProc;
		this->cbClsExtra = 0;
		this->cbWndExtra = 0;
		this->hInstance = Windows::GetInstanceHandle();
		this->hIcon = NULL;
		this->hCursor = ::LoadCursor(NULL, IDC_ARROW);
		this->hbrBackground = NULL;
		this->lpszMenuName = nullptr;
		this->lpszClassName = L"YupeiWindow";
		this->hIconSm = NULL;
		this->hbrBackground = NULL;
	}

	NativeWindow::NativeWindow(RenderType type)
		:ContentControl()
	{
		context = nullptr;
		//backgroundColor = 0xFFFFFFFF;
		renderType = type;
		Create(&NativeWindowProperties());
	}

	NativeWindow::NativeWindow(NativeWindowProperties * properties, RenderType type)
		:ContentControl()
	{
		renderType = type;
		Create(properties);
	}

	void NativeWindow::Register(const WindowClassEx * windowClass) const
	{
		if (::FindAtom(windowClass->lpszClassName) == 0)
		{
			::AddAtom(windowClass->lpszClassName);
			windowClass->Register();
		}
	}

	void NativeWindow::Create(NativeWindowProperties * nativeWindowProperties)
	{
		//auto nativeWindowProperties = dynamic_cast<NativeWindowProperties*>(windowProperties);

		Register(nativeWindowProperties->WindowClass.get());
		createArgs = new WindowCreateArgs(this);
		windowHandle = ::CreateWindowEx(
			nativeWindowProperties->ExStyle,
			nativeWindowProperties->WindowClass->lpszClassName,
			nativeWindowProperties->WindowTitle.data(),
			nativeWindowProperties->WindowStyle,
			nativeWindowProperties->X,
			nativeWindowProperties->Y,
			nativeWindowProperties->Width,
			nativeWindowProperties->Height,
			nativeWindowProperties->ParentHandle,
			nativeWindowProperties->MenuHandle,
			nativeWindowProperties->InstanceHandle,
			static_cast<void*>(this));
		InitializeRendering();
		switch (renderType)
		{
		case Yupei::WindowBase::RenderType::Hardware:
			backgroudBrush = Graphics::Direct2D::SolidColorBrush::GetWhiteBrush();
			break;
		case Yupei::WindowBase::RenderType::Software:
			//Software brush
			break;
		default:
			break;
		}
		InitializeArgs();
		paintArgs->context = this->context;
	}

	void NativeWindow::Run()
	{
		::ShowWindow(windowHandle, WindowShownMethod::FirstShown);
		::UpdateWindow(windowHandle);
		DWORD nowTime , tempTime = ::GetTickCount();
		//while (true)
		//{
		//	if ((nowTime = ::GetTickCount()) - tempTime >= PaintInterval)
		//	{
		//		tempTime = nowTime;
		//		::UpdateWindow(windowHandle);
		//	}
		//	if (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE))
		//	{
		//		if (messages.message == WM_QUIT) break;
		//		else
		//		{
		//			TranslateMessage(&messages);
		//			DispatchMessage(&messages);
		//		}
		//	}
		//	//else ::UpdateWindow(windowHandle);
		//	
		//}
		while (::GetMessage(&messages,NULL,0,0))
		{
			TranslateMessage(&messages);
			DispatchMessage(&messages);
		}
		//return messages.wParam;
	}

	UnicodeString NativeWindow::GetTitle() const
	{
		const size_t BufferSize = GetWindowTextLength(windowHandle);
		UnicodeString str;
		str.resize(BufferSize + 1u);
		GetWindowText(windowHandle, &str[0], str.size());
		return str;
	}

	void NativeWindow::InitializeRendering()
	{
		switch (renderType)
		{
		case RenderType::Hardware:
			context = new Graphics::Direct2D::DrawingContext(windowHandle);
			break;
		case RenderType::Software:
			//context = new Graphics::GdiDrawingContext(::GetDC(windowHandle));
			break;
		default:
			break;
		}
	}

	NativeWindow::~NativeWindow()
	{
		delete createArgs;
		delete paintArgs;
		delete ncPaintArgs;
		delete resizeArgs;
		delete context;
		
	}

	void NativeWindow::InitializeArgs()
	{
		paintArgs = new WindowPaintArgs(this, context);
	    ncPaintArgs = new WindowNCPaintArgs(this);
		resizeArgs = new WindowResizeArgs(this);
	}


}


