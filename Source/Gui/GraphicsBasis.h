#pragma once

#include <d2d1.h>
#include <d2d1helper.h>
#include <dwrite.h>
#include "Common.h"
#include "..\..\Source\Stl\Functional.h"

#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")

namespace Yupei
{
	/*Colors*/
	using Color = D2D1::ColorF;

	namespace Graphics
	{
		class IStrokeStyle
		{

		};
		struct Point : public D2D1_POINT_2F
		{
			Point(float X = 0.f, float Y = 0.f)
			{
				this->x = X;
				this->y = Y;
			}
			Point(const D2D1_POINT_2F& p)
			{
				x = p.x;
				y = p.y;
			}
			Point operator+(const Point& rhs) const
			{
				return Point(x + rhs.x, y + rhs.y);
			}
			Point& operator += (const Point& rhs)
			{
				x += rhs.x;
				y += rhs.y;
				return *this;
			}
			Point operator-(const Point& rhs) const
			{
				return Point(x - rhs.x, y - rhs.y);
			}
			Point& operator -= (const Point& rhs)
			{
				x -= rhs.x;
				y -= rhs.y;
				return *this;
			}
			Point operator /(float divisor)
			{
				return Point{ x / divisor,y / divisor };
			}
			Point& operator /=(float divisor)
			{
				x /= divisor;
				y /= divisor;
				return *this;
			}
		};
		struct Size : public D2D1_SIZE_F
		{
			Size()
			{
				this->width = 0;
				this->height = 0;
			}
			Size(float x, float y)
			{
				this->width = x;
				this->height = y;
			}
			operator D2D1_SIZE_U()
			{
				return D2D1::SizeU(static_cast<UINT32>(width),
					static_cast<UINT32>(height));
			}
		};
		struct Rect :public D2D1_RECT_F
		{
			Rect()
				:Rect(0.f, 0.f, 0.f, 0.f)
			{

			}

			Rect(float l, float t, float r, float b)
			{
				this->top = t;
				this->bottom = b;
				this->right = r;
				this->left = l;
			}


			Rect(const Point& leftTopCorner, float width, float height)
				:Rect(leftTopCorner.x, leftTopCorner.y, leftTopCorner.x + width, leftTopCorner.y + height)
			{

			}

			float GetWidth() const noexcept
			{
				return this->right - this->left;
			}

			float GetHeight() const noexcept
			{
				return this->bottom - this->top;
			}

			Point GetLeftCorner() const noexcept
			{
				return Point(left, top);
			}

			Point GetRightCorner() const noexcept
			{
				return Point(right, bottom);
			}

			Rect& MoveTo(const Point& p) noexcept
			{
				left += p.x;
				top += p.y;
				right += p.x;
				bottom += p.y;
				return *this;
			}

			Rect GetUnion(const Rect& rhs) noexcept
			{
				return Rect(
					max(left, rhs.left),
					max(top, rhs.top),
					min(right, rhs.right),
					min(bottom, rhs.bottom));
			}

			bool IsIn(const Point& p) const noexcept
			{
				return (p.x > left && p.x < right) &&
					(p.y > top && p.y < bottom);
				//static_cast<bool>(::PtInRect())
			}

			Size GetSize() const noexcept
			{
				return Size(static_cast<unsigned int>(right - left), static_cast<unsigned int>(bottom - top));
			}

		};

		struct Matrix3x2 : public D2D1::Matrix3x2F
		{
		public:
			//Whatever
			void ResetToIdentity() noexcept
			{
				_11 = 1.f;
				_12 = 0.f;
				_21 = 0.f;
				_22 = 1.f;
				_31 = 0.f;
				_32 = 0.f;
			}
			Matrix3x2()
			{
				ResetToIdentity();
			}
			operator Matrix3x2F () const
			{
				Matrix3x2F res = { _11,_12,_21,_22,_31,_32 };
				return res;
			}
			Matrix3x2& operator =(const Matrix3x2& rhs)
			{
				*(Matrix3x2F*)this = *(Matrix3x2F*)&rhs;
				return *this;
			}
			Matrix3x2& operator =(const Matrix3x2F& rhs)
			{
				*(Matrix3x2F*)this = rhs;
				return *this;
			}
		};

		class IBrush
		{
		public:
			virtual float GetOpacity() const = 0;
			virtual Matrix3x2 GetTransform() const = 0;
			virtual void SetOpacity(float) = 0;
			virtual void SetTransform(const Matrix3x2& matrix) = 0;
		};

		class ISolidColorBrush : public virtual IBrush
		{
		public:
			virtual Color GetColor() const noexcept = 0;
			virtual void SetColor(const Color& c) = 0;
		};

		class IGeometry;

		class IDrawingContext
		{
		public:
			virtual void BeginDraw() = 0;
			virtual void Clear(const Color& color) = 0;
			virtual void EndDraw() = 0;
			virtual void DrawRectangle(const Rect& rect, IBrush* brush, float width) = 0;
			virtual void FillRectangle(const Rect& rect, IBrush* brush) = 0;
			virtual std::pair<float, float> GetDpi() const = 0;
			virtual ISolidColorBrush* CreateSolidColorBrush(const Color& c) = 0;
			virtual void Resize(const Size& _size) = 0;
			virtual void DrawGeometry(IGeometry* geo, IBrush* _brush, IStrokeStyle* stroke = nullptr) = 0;
			virtual void SetTransform(const Matrix3x2* _trans) = 0;
			virtual Matrix3x2 GetTransform() const noexcept = 0;
			virtual void PushAxisAlignedClip(const Rect& _area) = 0;
			virtual void PopAxisAlignedClip() = 0;
			virtual ~IDrawingContext() = default;
		};
	}
}
