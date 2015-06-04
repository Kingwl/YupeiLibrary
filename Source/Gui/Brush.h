#pragma once

#include "GraphicsBasis.h"
#include "Common.h"
#include "WindowBase.h"

namespace Yupei
{
	namespace Graphics
	{
		namespace Direct2D
		{
			class BrushBase :public virtual IBrush
			{
			public:
				BrushBase()
					:IBrush(),
					brush_(nullptr)
				{
				}
				BrushBase(ID2D1Brush* _brush)
				{
					brush_ = _brush;
				}
				BrushBase(const BrushBase& rhs)
				{
					brush_ = rhs.brush_;
					brush_->AddRef();
				}
				BrushBase(BrushBase&& rhs)
				{
					brush_ = rhs.brush_;
					rhs.brush_ = nullptr;
				}
				virtual ~BrushBase()
				{
					if (brush_)
						brush_->Release();
				}
				float GetOpacity() const override
				{
					return brush_->GetOpacity();
				}
				void SetOpacity(float o)  override 
				{  
					brush_->SetOpacity(o); 
				}
				Matrix3x2 GetTransform() const override
				{
					Matrix3x2 res;
					brush_->GetTransform(&res);
					return res;
				}
				void SetTransform(const Matrix3x2& matrix) override 
				{ 
					brush_->SetTransform(matrix); 
				}
				ID2D1Brush* GetNativeBrush() const noexcept 
				{ 
					return brush_; 
				}
			protected:
				ID2D1Brush* brush_;
			};

			class DrawingContext;

			class SolidColorBrush :public BrushBase, public ISolidColorBrush
			{
			public:
				SolidColorBrush(const Color& c,DrawingContext* context);
				static SolidColorBrush* GetBlackBrush() noexcept
				{
					return blackBrush.get();
				}
				static SolidColorBrush* GetWhiteBrush() noexcept
				{
					return whiteBrush.get();
				}
				static SolidColorBrush* GetRedBrush() noexcept
				{
					return redBrush.get();
				}
				static SolidColorBrush* GetGreenBrush() noexcept
				{
					return greenBrush.get();
				}
				static SolidColorBrush* GetBlueBrush() noexcept
				{
					return blueBrush.get();
				}
				Color GetColor() const noexcept override
				{
					auto tempColor = reinterpret_cast<ID2D1SolidColorBrush*>(GetNativeBrush())->GetColor();
					return Color{ tempColor.r,tempColor.g,tempColor.b,tempColor.a };
				}
				void SetColor(const Color& c) override
				{
					reinterpret_cast<ID2D1SolidColorBrush*>(GetNativeBrush())->SetColor(c);
				}
				static void InitializeAllBrushes(DrawingContext* context);
			private:
				static unique_ptr<SolidColorBrush> blackBrush;
				static unique_ptr<SolidColorBrush> whiteBrush;
				static unique_ptr<SolidColorBrush> redBrush;
				static unique_ptr<SolidColorBrush> greenBrush;
				static unique_ptr<SolidColorBrush> blueBrush;
				static bool isInitialized;
			};

			class LinearGradientBrush : public BrushBase
			{
			public:
				LinearGradientBrush(D2D1_GRADIENT_STOP * stops,
					UINT stopCount,
					D2D1_POINT_2F startPaoint,
					D2D1_POINT_2F endPoint,
					DrawingContext * context);
			};
		}
		

		
	}
}