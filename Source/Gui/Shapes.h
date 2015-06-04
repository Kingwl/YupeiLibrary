#pragma once

#include "UIElement.h"

namespace Yupei
{
	class Shapes : public UIElement
	{
	public:
		Shapes() = default;
		Shapes(float _width, float _height )
			:UIElement(_width,_height)
		{

		}
		void AddVisualChild(Visual* v)
		{

		}
		void SetFill(Graphics::IBrush* _brush)
		{
			fillBrush = _brush;
		}
		Graphics::IBrush* GetFill() const noexcept
		{
			return fillBrush;
		}
		void SetStroke(Graphics::IBrush* _brush) noexcept
		{
			strokeBrush = _brush;
		}
		Graphics::IBrush* GetStroke() const noexcept
		{
			return strokeBrush;
		}
	protected:
		Graphics::IBrush* fillBrush = nullptr;
		Graphics::IBrush* strokeBrush = nullptr;
	};

	class Rectangle : public Shapes
	{
	public:
		Rectangle(const Graphics::Rect& _rect)
		{
		}
		Rectangle(float _width, float _height)
			:Shapes(_width,_height)
		{
		}
		Rectangle() = default;
		void RenderInternal(Graphics::IDrawingContext* context) override
		{
			context->FillRectangle(GetVirtualPosition(), fillBrush);
		}
		bool HitTest(const Graphics::Point& point) const noexcept
		{
			return GetRealPosition().IsIn(renderTransform->GetOrignalPoint(point));
		}
	};
	class Path : public Shapes
	{
	public:
		Path()
			:Shapes()
		{

		}
		Graphics::IGeometry* GetData() const noexcept
		{
			return data;
		}
		void SetData(Graphics::IGeometry* geo)
		{
			data = geo;
			//SetWidth(geo->GetBounds().GetWidth());
		}
		void OnRender(Graphics::IDrawingContext* context) override
		{
			context->DrawGeometry(data, fillBrush);
		}
		bool HitTest(const Graphics::Point& point) const noexcept
		{
			return data->FillContainsPoint(renderTransform->Get(), point);
			//|| Stroke things
		}
	private:
		Graphics::IGeometry* data;
	};


};
