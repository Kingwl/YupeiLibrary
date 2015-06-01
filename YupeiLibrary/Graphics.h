#pragma once

#include "Common.h"
#include "Observers.h"
#include "GraphicsBasis.h"
#include "WindowBase.h"
#include "Brush.h"

namespace Yupei
{
	struct IAnimation;
	namespace Graphics
	{
		enum class GeometryRelation
		{
			Unknown = 0,
			Disjoint = 1,
			IsContained = 2,
			Contains = 3,
			Overlap = 4
		};

		class IGeometry
		{
		public:
			virtual float ComputeLength(const Matrix3x2* matrix) const = 0;
			virtual float ComputeArea(const Matrix3x2* matrix) const = 0;
			virtual pair<Point, Point> ComputePointAtLength(float length, const Matrix3x2 * matrix) const = 0;
			virtual bool FillContainsPoint(const Matrix3x2* matrix, const Point& p) const = 0;
			virtual Graphics::Rect GetWidenedBounds(const Matrix3x2* matrix, const IStrokeStyle* stroke)const = 0;
			virtual Graphics::Rect GetBounds(const Matrix3x2* matrix) const = 0;
			virtual GeometryRelation CompareWithGeometry(const IGeometry*, const Matrix3x2* matrix) const = 0;
		};

		class IRectangleGeometry : public virtual IGeometry
		{
		public:
			virtual Rect GetRect() const noexcept = 0;
			virtual void SetRect(const Rect& r) = 0;
		};

		inline void MakeTranslateMatrix(const Point& p, Matrix3x2* matrix)
		{
			matrix->_11 = 1.0; matrix->_12 = 0.0;
			matrix->_21 = 0.0; matrix->_22 = 1.0;
			matrix->_31 = p.x; matrix->_32 = p.y;
		}

		inline void MakeScaleMatrix(float _scaleX, float _scaleY,
			const Point& _centre, Matrix3x2* matrix)
		{
			matrix->_11 = _scaleX; matrix->_12 = 0.0;
			matrix->_21 = 0.0; matrix->_22 = _scaleY;
			matrix->_31 = _centre.x - _scaleX * _centre.x;
			matrix->_32 = _centre.y - _scaleY * _centre.y;
		}

		inline Point RestorePoint(Matrix3x2 _matrix, const Point& _p)
		{
			Point res;
			D2D1InvertMatrix(&_matrix);
			res.x = (_p.x * _matrix._11) + (_p.y * _matrix._21) + _matrix._31;
			res.y = (_p.x * _matrix._12) + (_p.y * _matrix._22) + _matrix._32;
			return res;
		}

		struct TransformBase
		{
		public:
			TransformBase()
			{
				matrix = new Matrix3x2();
			}
			virtual ~TransformBase()
			{
				delete matrix;
			}
			//得到变换后的矩阵
			virtual Matrix3x2* Get() noexcept = 0;
			//将使用该矩阵变换后的点还原
			virtual Point GetOrignalPoint(const Point& _p) noexcept = 0;
		protected:
			Matrix3x2* matrix;
		};

		struct TranslateTransform : public TransformBase
		{
		private:
			Point translateSize;
		public:
			TranslateTransform(const Point& _p)
				:translateSize(_p)
			{

			}
			void SetTranslatePoint(const Point& p) noexcept
			{
				translateSize = p;
			}
			Matrix3x2* Get() noexcept override
			{
				MakeTranslateMatrix(translateSize, matrix);
				return matrix;
			}
			Point GetOrignalPoint(const Point& _p) noexcept override
			{
				return Point{ _p.x - translateSize.x,_p.y - translateSize.y };
			}
		};

		struct RotateTransform : public TransformBase
		{
			float angle = 0.0;
			Graphics::Point rotateCentre;
		public:
			RotateTransform(float _an, const Graphics::Point& _p = Point())
				:angle(_an), rotateCentre(_p)
			{

			}
			Matrix3x2* Get() noexcept override
			{
				D2D1MakeRotateMatrix(angle, rotateCentre, matrix);
				return matrix;
			}
			Point GetOrignalPoint(const Point& _p) noexcept override
			{
				float temp = sqrt(
					pow(_p.x - rotateCentre.x, 2.f) +
					pow(_p.y - rotateCentre.y, 2.f)
					);
				return Point{
					rotateCentre.x + temp * cos(angle),
					rotateCentre.y + temp * sin(angle)
				};
			}
		};

		struct ScaleTransform :public TransformBase
		{
			float scaleX = 1.f;
			float scaleY = 1.f;
			Point scaleCentre;
			ScaleTransform(float _scaleX, float _scaleY, const Point& p = Point{})
				:scaleX(_scaleX),
				scaleY(_scaleY),
				scaleCentre(p)
			{

			}
			Matrix3x2* Get() noexcept override
			{
				MakeScaleMatrix(scaleX, scaleY, scaleCentre, matrix);
				return matrix;
			}
			Point GetOrignalPoint(const Point& _p) noexcept override
			{
				return Point{
					_p.x * 1.f / scaleX,
					_p.y * 1.f / scaleY
				};
			}
		};
		struct MatrixTransform : public TransformBase
		{
			Matrix3x2* Get() noexcept override
			{
				return matrix;
			}
			void SetMatrix(Matrix3x2* _matrix) noexcept
			{
				matrix = _matrix;
			}
			Point GetOrignalPoint(const Point& _p) noexcept override
			{
				return RestorePoint(*matrix, _p);
			}
		};

		namespace Direct2D
		{
			class GraphicsFactory
			{
			public:
				static ID2D1Factory* GetD2DFactory() noexcept
				{
					return factoryPtr->d2dFactory;
				}
				static IDWriteFactory* GetDWriteFactory() noexcept
				{
					return factoryPtr->dwriteFactory;
				}
				static GraphicsFactory* GetGraphicsFactory()
				{
					if (factoryPtr == nullptr) factoryPtr = unique_ptr<GraphicsFactory>(new GraphicsFactory());
					return factoryPtr.get();
				}
				~GraphicsFactory()
				{
					d2dFactory->Release();
					dwriteFactory->Release();
				}
			private:
				GraphicsFactory();
				ID2D1Factory* d2dFactory;
				IDWriteFactory* dwriteFactory;
				static unique_ptr<GraphicsFactory> factoryPtr;
			};

			inline ID2D1Factory* GetD2DFactory()
			{
				return GraphicsFactory::GetGraphicsFactory()->GetD2DFactory();
			}

			inline IDWriteFactory* GetDWriteFactory()
			{
				return GraphicsFactory::GetGraphicsFactory()->GetDWriteFactory();
			}

			class GeometryBase : public virtual IGeometry
			{
			public:
				GeometryBase()
					:IGeometry(),
					geometry(nullptr)
				{
				}
				// Inherited via IGeometry
				virtual float ComputeLength(const Matrix3x2 * matrix) const override;
				virtual float ComputeArea(const Matrix3x2 * matrix) const override;
				virtual pair<Point, Point> ComputePointAtLength(float length, const Matrix3x2 * matrix) const override;
				virtual bool FillContainsPoint(const Matrix3x2 * matrix, const Point & p) const override;
				virtual Graphics::Rect GetWidenedBounds(const Matrix3x2 * matrix, const IStrokeStyle * stroke) const override;
				virtual Graphics::Rect GetBounds(const Matrix3x2 * matrix) const override;
				virtual GeometryRelation CompareWithGeometry(const IGeometry *, const Matrix3x2 * matrix) const override;

				ID2D1Geometry* GetNativeGeometry() const noexcept
				{
					return geometry;
				}
				virtual ~GeometryBase()
				{
					if (geometry) geometry->Release();
				}
			protected:
				ID2D1Geometry* geometry;
			};

			class RectangleGeometry : public IRectangleGeometry, public GeometryBase
			{
			public:
				RectangleGeometry()
					:IGeometry()
				{

				}
				RectangleGeometry(const Rect& r)
					:IGeometry()
				{
					InitializeGeometry(r);
				}
				void SetRect(const Rect& r) override
				{
					InitializeGeometry(r);
				}
				Rect GetRect() const noexcept
				{
					D2D1_RECT_F rect;
					reinterpret_cast<ID2D1RectangleGeometry*>(geometry)->GetRect(&rect);
					return Rect{ rect.left,rect.top,rect.right,rect.bottom };
				}
			private:
				void InitializeGeometry(const Rect& r)
				{
					if (geometry != nullptr) geometry->Release();
					GetD2DFactory()->CreateRectangleGeometry(
						r,
						reinterpret_cast<ID2D1RectangleGeometry**>(&geometry));
				}
			};

			class DrawingContext : public IDrawingContext
			{
			public:
				DrawingContext(HWND _windowHandle);
				std::pair<float, float> GetDpi() const override;
				void Resize(const Size& _size) override;

				void BeginDraw() override
				{
					renderTarget->BeginDraw();
				}
				void Clear(const Color& color) override
				{
					renderTarget->Clear(color);
				}
				void EndDraw() override
				{
					renderTarget->EndDraw();
				}
				void DrawRectangle(const Rect& rect, IBrush* brush, float width) override
				{
					if (brush)
						renderTarget->DrawRectangle(
							rect, dynamic_cast<BrushBase*>(brush)->GetNativeBrush(), width);
				}
				void FillRectangle(const Rect& rect, IBrush* brush) override
				{
					if (brush)
						renderTarget->FillRectangle(rect, dynamic_cast<BrushBase*>(brush)->GetNativeBrush());
				}
				ISolidColorBrush* CreateSolidColorBrush(const Color& c) override
				{
					return new Direct2D::SolidColorBrush(c, this);
				}
				LinearGradientBrush* GetLinearGradientBrush(D2D1_GRADIENT_STOP * stops,
					UINT stopCount,
					D2D1_POINT_2F startPaoint,
					D2D1_POINT_2F endPoint)
				{
					return new LinearGradientBrush(
						stops,
						stopCount,
						startPaoint,
						endPoint,
						this);
				}
				ID2D1RenderTarget* GetNativeTarget() const noexcept
				{
					return renderTarget;
				}
				void SetTransform(const Matrix3x2* _trans) override
				{
					renderTarget->SetTransform(_trans);
				}
				Matrix3x2 GetTransform() const noexcept override
				{
					Matrix3x2 res;
					renderTarget->GetTransform(&res);
					return res;
				}
				~DrawingContext()
				{
					if (renderTarget) renderTarget->Release();
				}
				void DrawGeometry(IGeometry * geo, IBrush * _brush, IStrokeStyle * stroke = nullptr) override
				{
					renderTarget->DrawGeometry(
						reinterpret_cast<Direct2D::GeometryBase*>(geo)->GetNativeGeometry(),
						reinterpret_cast<Direct2D::BrushBase*>(_brush)->GetNativeBrush(),
						1.f,
						nullptr);
				}
				void PushAxisAlignedClip(const Rect& _area) override
				{
					renderTarget->PushAxisAlignedClip(_area, D2D1_ANTIALIAS_MODE_PER_PRIMITIVE);
				}
				void PopAxisAlignedClip()
				{
					renderTarget->PopAxisAlignedClip();
				}
			private:
				ID2D1HwndRenderTarget* renderTarget;
			};

		}

		namespace Gdi
		{
			/*class GdiDrawingContext : public IDrawingContext
			{
			public:
			GdiDrawingContext(HDC hDC);
			void BeginDraw() override;
			void Clear(const Color& color) override;
			void EndDraw() override;
			void DrawRectangle(const Rect& rect, IBrush* brush, float width) override;
			void FillRectangle(const Rect& rect, IBrush* brush) override;
			std::pair<float, float> GetDpi() const override;
			IBrush* GetBrush() const override;
			private:
			HDC dcHandle;
			};*/
			//class Yupei::IAnimation;
		}
	}
};
