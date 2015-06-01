#pragma once

#include "UIElement.h"

namespace Yupei
{
	class Panel : public ContainerVisual, public UIElement
	{
	public:
		Panel()
			:Visual(),ContainerVisual(),UIElement()
		{
		}
		void SetBackground(Graphics::IBrush* brush) noexcept
		{
			backgroundBrush = brush;
		}
		Graphics::IBrush* GetBackground() const noexcept
		{
			return backgroundBrush;
		}
		void AddLogicalChild(UIElement* ui)
		{
			assert(ui->GetLogicalParent() == nullptr);
			ui->SetLogicalParent(this);
			logicalChildren.push_back(ui);
		}
		vector<UIElement*>& GetLogicalChildren()
		{
			return logicalChildren;
		}
		const vector<UIElement*>& GetLogicalChildren() const
		{
			return logicalChildren;
		}
		
		virtual void ArrangeChildren(bool isIgnored)
		{
			for (auto&& _child : GetLogicalChildren())
			{
				_child->OnResize(false);
			}
		}
		bool HitTest(const Graphics::Point& point) const noexcept override
		{
			return GetRealPosition().IsIn(renderTransform->GetOrignalPoint(point));
		}
		/*IsIgnored为true表示不管“我”的HorizonalAlignment*/
		//正常的Panel先将自己排好版，接着让每个child自己排版
		//Canvas不同，自己排好版后它亲自给child排版，将Igorned设置为true，当
		//每个Canvas的Child看到Ignore为true后什么都不做，因为Parent已经给自己
		//排好版了
		void OnResize(bool isIgnored) override
		{
			UIElement::OnResize(isIgnored);
			ArrangeChildren(isIgnored);
		}
		
		void RenderInternal(Graphics::IDrawingContext* context) override;

	private:
		Graphics::IBrush* backgroundBrush = nullptr;
		vector<UIElement*> logicalChildren;
	};

	class Canvas : public Panel
	{
	public:
		static inline void SetX(UIElement* element, float _x) noexcept/*(noexcept(UIElement::SetCanvasX))*/
		{
			element->SetCanvasX(_x);
		}
		static inline void SetY(UIElement* element, float _y) noexcept/*(noexcept(UIElement::SetCanvasY))*/
		{
			element->SetCanvasY(_y);
		}
		float GetChildWidth(UIElement* _child) const noexcept override
		{
			return _child->GetActualWidth();
		}
		float GetChildHeight(UIElement* _child) const noexcept override
		{
			return _child->GetActualHeight();
		}

		void ArrangeChildren(bool isIgnored) override;
	};

	class StackPanel : public Panel
	{
	public:
		enum class OrientationType
		{
			Vertical,
			Horizontal
		};
		float GetChildWidth(UIElement* _child) const noexcept override
		{
			return _child->GetActualWidth();
		}
		float GetChildHeight(UIElement* _child) const noexcept override
		{
			return _child->GetActualHeight();
		}
		void ArrangeChildren(bool isIgnored) override;
	private:
		OrientationType orientetion = OrientationType::Vertical;
	};
}
