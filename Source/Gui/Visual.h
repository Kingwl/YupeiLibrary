#pragma once

#include "Graphics.h"
#include "Observers.h"

namespace Yupei
{
	class Visual
	{
	public:
		Visual()
			:visualParent()
		{
		}
		void SetVisualParent(Visual* p) noexcept
		{
			visualParent = p;
		}
		Visual* GetVisualParent() const noexcept
		{
			return visualParent;
		}
		virtual void AddVisualChild(Visual* v)
		{
			assert(v->GetVisualParent() == nullptr);
			v->SetVisualParent(this);
		}

		//virtual Graphics::Point PointToScreen(const Graphics::Point& point) const noexcept = 0;
		virtual bool HitTest(const Graphics::Point& point) const noexcept = 0;
		virtual void OnRender(Graphics::IDrawingContext* context) = 0;

	protected:
		Visual* visualParent;
		
	};
	//WPF中ContainerVisual并不指定具体的Container(?)

	class ContainerVisual : public virtual Visual
	{
	public:
		void AddVisualChild(Visual* v) override
		{
			Visual::AddVisualChild(v);
			visualChildren.push_back(v);
		}
		vector<Visual*>& GetVisualChildren()
		{
			return visualChildren;
		}
		const vector<Visual*>& GetVisualChildren() const
		{
			return visualChildren;
		}
		Visual* GetVisualChild(int index) const
		{
			return visualChildren[index];
		}
	protected:
		vector<Visual*> visualChildren;
	};

}
