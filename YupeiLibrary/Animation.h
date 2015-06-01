#pragma once

namespace Yupei
{
	struct IAnimation
	{
		virtual void Update() = 0;
		//virtual void
	};

	template<typename Type> 
	struct Animation : public IAnimation
	{
		Type GetState() const;
	};

	
}
