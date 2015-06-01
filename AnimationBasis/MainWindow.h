#pragma once

#include "Common.h"

class MainWindow : public Yupei::NativeWindow
{

public:
	MainWindow()
		:NativeWindow()
	{
		InitializeAnimationComponents();
		InitializeVariables();
		InitializeLayout();
		WindowPaintEvent += [this](Yupei::Object* sender, Yupei::WindowPaintArgs* args)
		{
			UI_ANIMATION_SECONDS secondsNow;
			this->animationTimer->GetTime(&secondsNow);
			this->animationManager->Update(secondsNow);
			double r = 0.0 , g= 0.0,b = 0.0;
			this->redAnimationVariable->GetValue(&r);
			this->greenAnimationVariable->GetValue(&g);
			this->blueAnimationVariable->GetValue(&b);
			dynamic_cast<Yupei::Graphics::Direct2D::SolidColorBrush*>(animationRect->GetFill())->SetColor(
				Yupei::Color{ static_cast<float>(r),
				static_cast<float>(g),
				static_cast<float>(b) });
		};
	}


private:
	const UI_ANIMATION_SECONDS Duration = 2.0;
	void InitializeAnimationComponents()
	{
		CoCreateInstance(
			CLSID_UIAnimationManager,
			nullptr,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&animationManager));
		CoCreateInstance(
			CLSID_UIAnimationTimer,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&animationTimer)
			);
		CoCreateInstance(
			CLSID_UIAnimationTransitionLibrary,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&transitionLibrary)
			);
		

	}
	void StoryboradHelper(IUIAnimationTransition*& trans,IUIAnimationVariable* var)
	{
		transitionLibrary->CreateAccelerateDecelerateTransition(
			Duration,
			1.0,
			0.5,
			0.5,
			&trans);
		storyboard->AddTransition(var, trans);
	}
	void InitializeLayout()
	{
		mainCanvas = new Yupei::Canvas();
		this->SetContent(mainCanvas);
		animationRect = new Yupei::Rectangle{ 150.f,150.f };
		animationRect->SetFill(Yupei::Graphics::Direct2D::SolidColorBrush::GetGreenBrush());
		mainCanvas->AddLogicalChild(animationRect);
		mainCanvas->AddVisualChild(animationRect);
		Yupei::Canvas::SetX(animationRect, 100.f);
		Yupei::Canvas::SetY(animationRect, 100.f);
	}
	void InitializeVariables()
	{
		InitializeVariableHelper(redAnimationVariable);
		InitializeVariableHelper(greenAnimationVariable);
		InitializeVariableHelper(blueAnimationVariable);
		animationManager->CreateStoryboard(&storyboard);
		StoryboradHelper(redTransition, redAnimationVariable);
		StoryboradHelper(blueTransition, blueAnimationVariable);
		StoryboradHelper(greenTransition, greenAnimationVariable);
		UI_ANIMATION_SECONDS secondsNow;
		animationTimer->GetTime(
			&secondsNow
			);
		storyboard->Schedule(
			secondsNow
			);
	}

	void InitializeVariableHelper(IUIAnimationVariable*& var)
	{
		animationManager->CreateAnimationVariable(
			0.0,
			&var);
		//var->SetLowerBound(0.0);
		//var->SetUpperBound(255.0);
	}
	IUIAnimationManager* animationManager = nullptr;
	IUIAnimationTimer* animationTimer = nullptr;
	IUIAnimationTransitionLibrary* transitionLibrary = nullptr;
	Yupei::Canvas* mainCanvas = nullptr;
	Yupei::Rectangle* animationRect = nullptr;
	IUIAnimationVariable* redAnimationVariable = nullptr;
	IUIAnimationVariable* greenAnimationVariable = nullptr;
	IUIAnimationVariable* blueAnimationVariable = nullptr;
	IUIAnimationStoryboard* storyboard = nullptr;
	IUIAnimationTransition* redTransition = nullptr;
	IUIAnimationTransition* greenTransition = nullptr;
	IUIAnimationTransition* blueTransition = nullptr;
};