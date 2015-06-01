#include "Brush.h"
#include "Graphics.h"

namespace Yupei
{
	namespace Graphics
	{
		namespace Direct2D
		{
			SolidColorBrush::SolidColorBrush(const Color & c, DrawingContext* context)
				:IBrush()
			{
				context->GetNativeTarget()->CreateSolidColorBrush(
					c, reinterpret_cast<ID2D1SolidColorBrush**>(&brush_));
			}
			void SolidColorBrush::InitializeAllBrushes(DrawingContext* context)
			{
				if (!isInitialized)
				{
					SolidColorBrush::blackBrush = make_unique<SolidColorBrush>(D2D1::ColorF::Black, context);
					SolidColorBrush::whiteBrush = make_unique<SolidColorBrush>(D2D1::ColorF::White, context);
					SolidColorBrush::redBrush = make_unique<SolidColorBrush>(D2D1::ColorF::Red, context);
					SolidColorBrush::greenBrush = make_unique<SolidColorBrush>(D2D1::ColorF::Green, context);
					SolidColorBrush::blueBrush = make_unique<SolidColorBrush>(D2D1::ColorF::Blue, context);
					isInitialized = true;
				}
			}
			unique_ptr<SolidColorBrush> SolidColorBrush::blackBrush = nullptr;
			unique_ptr<SolidColorBrush> SolidColorBrush::whiteBrush = nullptr;
			unique_ptr<SolidColorBrush> SolidColorBrush::redBrush = nullptr;
			unique_ptr<SolidColorBrush> SolidColorBrush::greenBrush = nullptr;
			unique_ptr<SolidColorBrush> SolidColorBrush::blueBrush = nullptr;
			bool SolidColorBrush::isInitialized = false;


			LinearGradientBrush::LinearGradientBrush(D2D1_GRADIENT_STOP * stops,
				UINT stopCount,
				D2D1_POINT_2F startPaoint,
				D2D1_POINT_2F endPoint,
				DrawingContext * context)
			{
				ID2D1GradientStopCollection* collection = nullptr;
				auto target = context->GetNativeTarget();
				target->CreateGradientStopCollection(
					stops,
					stopCount,
					D2D1_GAMMA_2_2,
					D2D1_EXTEND_MODE_CLAMP,
					&collection);
				target->CreateLinearGradientBrush(
					D2D1::LinearGradientBrushProperties(
						startPaoint, endPoint),
					collection,
					reinterpret_cast<ID2D1LinearGradientBrush**>(&brush_)
					);
				collection->Release();
			}
		}
		
	}
}




