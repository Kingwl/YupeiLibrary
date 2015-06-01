#include "UIElement.h"

namespace Yupei
{
	float UIElement::GetDesiredWidth() const noexcept
	{
		if (layoutTransform != nullptr)
		{
			auto leftTop = leftCornerPos;
			auto rightBottom = Graphics::Point{ GetLeftCornerX() + actualWidth ,GetLeftCornerY() + actualHeight };
			leftTop = layoutTransform->Get()->TransformPoint(leftTop);
			rightBottom = layoutTransform->Get()->TransformPoint(rightBottom);
			return abs(leftTop.x - rightBottom.x);
		}
		else return actualWidth;
		
	}
	float UIElement::GetDesiredHeight() const noexcept
	{
		if (layoutTransform != nullptr)
		{
			auto leftButtom = Graphics::Point{ GetLeftCornerX(),GetLeftCornerY() + actualHeight };
			auto rightTop = Graphics::Point{ GetLeftCornerX() + actualWidth,GetLeftCornerY() };
			leftButtom = layoutTransform->Get()->TransformPoint(leftButtom);
			rightTop = layoutTransform->Get()->TransformPoint(rightTop);
			return abs(leftButtom.y - rightTop.y);
		}
		else return actualHeight;
	}
	//UIElement的OnResize默认调用LayoutInternal
	//LayoutInternal默认调用ProcessWidth与ProcessHeight
	//
	void UIElement::ProcessWidth() noexcept
	{
		if (!std::isnan(width)) //如果width是NaN，说明用户没有设置宽度。如果设置了，使actualWidth为width
		{
			SetActualWidth(width);
		}
		if (logicalParent != nullptr)
		{
			switch (horizontalAlignment)
			{
			case HorizontalAlignmentType::Left:
			{
				SetLeftCornerX(logicalParent->GetLeftCornerX());
				break;
			}
			case HorizontalAlignmentType::Right:
			{
				SetLeftCornerX(logicalParent->GetLeftCornerX() + logicalParent->GetChildWidth(this)//the right pos of parent
					- GetActualWidth()); //my pos
				break;
			}
			case HorizontalAlignmentType::Centre:
			{
				SetLeftCornerX(logicalParent->GetLeftCornerX() + logicalParent->GetChildWidth(this) / 2.f
					- GetActualWidth());
				break;
			}
			case HorizontalAlignmentType::Stretch:
			{
				SetLeftCornerX(logicalParent->GetLeftCornerX());
				SetActualWidth(logicalParent->GetChildWidth(this));
				int i = 0;
				break;
			}
			default:
				break;
			}
		}
	}
	void UIElement::ProcessHeight() noexcept
	{
		if (!std::isnan(height))
		{
			SetActualHeight(height);
		}
		if (logicalParent != nullptr)
		{
			switch (verticalAlignment)
			{
			case VerticalAlignmentType::Top:
			{
				SetLeftCornerY(logicalParent->GetLeftCornerY());
				break;
			}
			case VerticalAlignmentType::Bottom:
			{
				SetLeftCornerY(logicalParent->GetLeftCornerY() + logicalParent->GetChildHeight(this)//the right pos of logicalParent
					- GetActualHeight()); //my pos
				break;
			}
			case VerticalAlignmentType::Centre:
			{
				SetLeftCornerY(logicalParent->GetLeftCornerY() + logicalParent->GetChildHeight(this) / 2.f
					- GetActualHeight());
				break;
			}
			case VerticalAlignmentType::Stretch:
			{
				SetLeftCornerY(logicalParent->GetLeftCornerY());
				SetActualHeight(logicalParent->GetChildHeight(this));
				break;
			}
			default:
				break;
			}
		}
	}
}

