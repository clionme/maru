#pragma once

/*

Stores pre-formulated unit movements

 Aimed for being used by AI module.

*/

#include "DataTypes.h"

class battleMovePreset
{
	double d; //dummy variable

public:
	battleMovePreset(void);
	~battleMovePreset(void);

	void Mv_to_TargetU_Guard(Unit*); //guards secondary target

	void Mv_to_TargetU(Unit*,double); //move to target unit
								//can also be used as charge
	void Mv_dist_TargetU(Unit*,double); //maintains distance from target unit
								//can also be used as dodge or run-away
	void Mv_to_Position(Unit*,dPOINT*); //move to target position
	void Mv_circulate_Position(Unit *u, double, bool); //circulates the position

	void Mv_zigzag_TargetU(Unit*,double, bool); //moves zigzag towards target - for avoiding arrows

	void Mv_diagaway_TargetU(Unit*, bool);
	void Mv_diag_TargetU(Unit*, bool);

	void Mv_circulate_TargetU(Unit *u, double, bool); //circulates the target 
								//in clock, or in counter clockwise direction

	void Mv_DA(Unit*); //death animation
};

