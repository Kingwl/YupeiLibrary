#include "Panel.h"

namespace Yupei
{
	void Panel::RenderInternal(Graphics::IDrawingContext * context)
	{
		context->FillRectangle(GetVirtualPosition(), backgroundBrush);
		for (auto&& child : GetVisualChildren())
		{
			child->OnRender(context);
		}
	}

	void Canvas::ArrangeChildren(bool isIgnored)
	{
		for (auto&& _child : GetLogicalChildren())
		{
			_child->SetLeftCorner(GetLeftCorner() + Graphics::Point{
				_child->GetCanvasX(),_child->GetCanvasY() });
			_child->SetActualWidth(_child->GetWidth());
			_child->SetActualHeight(_child->GetHeight());
			_child->OnResize(true);
			//给孩子们确定位置
		}
	}
	void StackPanel::ArrangeChildren(bool isIgnored)
	{
		if (orientetion == OrientationType::Vertical)
		{
			float startHeight = leftCornerPos.y;
			for (auto&& _child : GetLogicalChildren())
			{
				_child->OnResize(false);//先自己处理好
				_child->SetLeftCornerY(startHeight);
				startHeight += _child->GetDesiredHeight();
			}
			actualHeight = startHeight;
		}
		else
		{
			float startWidth = leftCornerPos.y;
			for (auto&& _child : GetLogicalChildren())
			{
				_child->OnResize(false);
				_child->SetLeftCornerX(startWidth);
				startWidth += _child->GetDesiredWidth();
			}
			actualWidth = startWidth;
		}
	}
}


