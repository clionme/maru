#pragma once

/*

Stores pre-formulated weapon movements

 Aimed for being used by AI module.

*/

#include "DataTypes.h"

#define CALL_uAI (u->AI_timer = AI_period-2)

#define PULL_out if ( u->wep.pulled > 0 ) u->wep.pulled -= u->wep.WD->length* u->wep.WD->pull_speed
#define PULL_in  if ( u->wep.pulled < u->wep.WD->length *  u->wep.WD->pull_max ) u->wep.pulled += u->wep.WD->length *  u->wep.WD->pull_speed;//0.2;

 class battleWeaponActionPreset
{
	double angle_between(double, double, double, double);
	double d, len;

public:
	battleWeaponActionPreset(void);
	~battleWeaponActionPreset(void);

	void Atk_Swing(Unit*); // random swing
	void Atk_HSwing(Unit*);// half swing
	void Atk_FSwing(Unit*);// full swing
	void Atk_Spin(Unit*);// spin
	void Atk_DashNSlash(Unit*);  // DashNSlash
	void Atk_Poke(Unit*);  // simple poke

	void Rdy_Guard(Unit *);//, Unit *); 

	void Rdy_Idle(Unit *);
	void Rdy_Idle_UserUnit(Unit *, bool, bool);
	void Rdy_Pull(Unit*);
	void Rdy_TanAnglePos(Unit *, bool, bool, int);

	inline void MoveWep(Unit *);

	inline void BoundWep(Unit*);
	inline void BoundWepSpeed(Unit*);
};