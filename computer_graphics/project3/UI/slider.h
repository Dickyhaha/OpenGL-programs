#ifndef _SLIDER_H_
#define _SLIDER_H_

#include "../Core/world_define.h"
#include "interface.h"
#include "../Core/world.h"
#include "../Core/pixel.h"

class Slider: public Interface
{
	private:	
		int MinValue;
		int MaxValue;

		int DefaultValue;
		int CurrentValue;

		bool Border;

	public:
		Slider(int Min, int Max, int Default, bool B = false);
		~Slider();

		int GetMaxValue();
		void SetMaxValue(int NewValue);

		int GetMinValue();
		void SetMinValue(int NewValue);

		int GetCurrentValue();
		void SetCurrentValue(int NewValue);

		int GetDefaultValue();
		void SetDefaultValue(int NewValue);

		virtual void Refresh();
};

#endif
