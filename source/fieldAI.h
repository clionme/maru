#pragma once
#include "DataTypes.h"
#include <math.h>

/*

The AI class for field control.

manipulates LGroup[]

*/

class fieldAI
{
public:
	fieldAI(void);
	~fieldAI(void);

	void simple_Move_To_AL(LGroup*, LGroup*);
	void simple_Move_To_Closest(LGroup*);

	void LGroup_Adjust_march_destination(LGroup*);
};

