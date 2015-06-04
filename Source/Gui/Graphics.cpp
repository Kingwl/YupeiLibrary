#include "Graphics.h"
#include "Animation.h"

namespace Yupei
{
	namespace Graphics
	{
		namespace Direct2D
		{
			unique_ptr<GraphicsFactory> GraphicsFactory::factoryPtr = nullptr;

			GraphicsFactory::GraphicsFactory()
			{
				D2D1CreateFactory(
					D2D1_FACTORY_TYPE_SINGLE_THREADED, &d2dFactory);
				DWriteCreateFactory(
					DWRITE_FACTORY_TYPE_SHARED,
					__uuidof(IDWriteFactory),
					reinterpret_cast<IUnknown**>(&dwriteFactory)
					);
			}

			DrawingContext::DrawingContext(HWND _windowHandle)
			{
				RECT rc;
				GetClientRect(_windowHandle, &rc);
				GetD2DFactory()->CreateHwndRenderTarget(
					D2D1::RenderTargetProperties(),
					D2D1::HwndRenderTargetProperties(
						_windowHandle,
						D2D1::SizeU(
							rc.right - rc.left,
							rc.bottom - rc.top)
						),
					reinterpret_cast<ID2D1HwndRenderTarget**>(&renderTarget)
					);
				SolidColorBrush::InitializeAllBrushes(this);
			}

			void DrawingContext::Resize(const Size & _size)
			{
				renderTarget->Resize(D2D1::SizeU(
					static_cast<UINT32>(_size.width),
					static_cast<UINT32>(_size.height)));
			}

			std::pair<float, float> DrawingContext::GetDpi() const
			{
				std::pair<float, float> res;
				renderTarget->GetDpi(&res.first, &res.second);
				return res;
			}
			float GeometryBase::ComputeLength(const Matrix3x2 * matrix) const
			{
				float res;
				geometry->ComputeLength(matrix, &res);
				return res;
			}
			float GeometryBase::ComputeArea(const Matrix3x2 * matrix) const
			{
				float res;
				geometry->ComputeArea(matrix, &res);
				return res;
			}
			pair<Point, Point> GeometryBase::ComputePointAtLength(float length, const Matrix3x2 * matrix) const
			{
				pair<Point, Point> res;
				geometry->ComputePointAtLength(length, matrix, &res.first, &res.second);
				return res;
			}
			bool GeometryBase::FillContainsPoint(const Matrix3x2 * matrix, const Point & p) const
			{
				BOOL res;
				geometry->FillContainsPoint(p, matrix, &res);
				return res;
			}
			Graphics::Rect GeometryBase::GetWidenedBounds(const Matrix3x2 * matrix, const IStrokeStyle * stroke) const
			{
				Rect res;
				geometry->GetWidenedBounds(
					0.f,
					nullptr,
					matrix,
					&res);
				return res;
			}
			Graphics::Rect GeometryBase::GetBounds(const Matrix3x2 * matrix) const
			{
				Rect res;
				geometry->GetBounds(
					matrix,
					&res);
				return res;
			}
			GeometryRelation GeometryBase::CompareWithGeometry(const IGeometry * rhs, const Matrix3x2 * matrix) const
			{
				D2D1_GEOMETRY_RELATION res;
				geometry->CompareWithGeometry(
					reinterpret_cast<const GeometryBase*>(rhs)->GetNativeGeometry(),
					matrix,
					&res);
				return GeometryRelation(res);
			}
		}
		
		namespace Gdi
		{
			/*void GdiDrawingContext::BeginDraw()
			{

			}
			void GdiDrawingContext::Clear(const Color & color)
			{
			}
			void GdiDrawingContext::EndDraw()
			{
			}
			void GdiDrawingContext::DrawRectangle(const Rect & rect, IBrush * brush, float width)
			{
			}
			void GdiDrawingContext::FillRectangle(const Rect & rect, IBrush * brush)
			{
			}*/

		}
	}
}

