#pragma once

#include "UIElement.h"
#include "Font.h"

namespace Yupei
{
	class Control : public UIElement
	{
	public:
		Control() = default;
		void SetBackground(Graphics::IBrush* _brush)
		{
			backgroudBrush = _brush;
		}
	protected:
		UnicodeString fontFamily = UnicodeString{ DefaultFontFamily };
		float fontSize = DefaultFontSize;
		FontWeight fontWeight = DefaultFontWeight;
		Graphics::IBrush* backgroudBrush = nullptr;
	};

	class ContentControl : public Control
	{
	public:
		ContentControl() = default;
		void OnRender(Graphics::IDrawingContext* dc) override
		{
			content->OnRender(dc);
		}
		void SetContent(UIElement* _content) noexcept
		{
			assert(_content->GetLogicalParent() == nullptr);
			assert(_content->GetVisualParent() == nullptr);
			_content->SetVisualParent(this);
			_content->SetLogicalParent(this);
			content = _content;
		}
		UIElement* GetContent() const noexcept
		{
			return content;
		}
		void OnResize(bool isIgnored) override
		{
			UIElement::OnResize(isIgnored);
			content->OnResize(isIgnored);
		}
	protected:
		UIElement* content;
	};

	class Run
	{
		Run(UnicodeString str)
			:contentString(str)
		{

		}
		void SetText(const UnicodeString& str)
		{
			contentString = str;
			*isChanged = true;
		}
		void SetIsChanged(bool* _p) noexcept
		{
			isChanged = _p;
		}
	private:
		UnicodeString contentString;
		bool* isChanged = nullptr;
	};


	class TextBlock : public UIElement
	{
	private:
		bool isChanged = false;

	};//No Focus...
}
