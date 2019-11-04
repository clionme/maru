#pragma once

#include "battleWeaponActionPreset.h"
#include "battleMovePreset.h"
#include "DataTypes.h"

/*

A battle engine...

Handles user / AI input

 and 

 processes unit-level battle field 

*/
#define is_atk(u) (( fabs(u.wep.angle_speed) > 0.1 ) || ( u.wep.Wep_status == Wep_on_poke ) || (u.wep.Wep_status == Wep_on_dns && u.wep.stance == 1) ) 
                  // || (u.wep.weapon_type == 4)  used when the dagger was too short ( auto hit )
class battleEE
{

	double dummy;
	double Wep_Wep_Collision_pushback;

	battleWeaponActionPreset	bWP;
	battleMovePreset			bMP;

	HitEffect *hef;
	int *next_effect_counter;

	bool *LButtonDown;
	bool *RButtonDown;

	Arrow *bEE_arw;
	int *arw_counter;
	double speed_modifier;

public:
	battleEE(int *, HitEffect*, bool*, bool*, Arrow *, int*);
	battleEE(void){};
	~battleEE(void);

	void CalcStamina(Unit *);
	void CollisionPoints(Unit *u);
	void CollisionDetection(Unit*, Unit*, Arrow *);
	
	void UnitCollisionChecker(Unit*, Unit*);

	void Move_UW(Unit *A, POINT);
	void Move_MW(Unit *);

	inline void Move_LoadedArrow(Unit*);

	void Chrg_Arrow(Arrow *);
	void Load_Arrow(Unit*, Arrow*, int);
	void Fire_Arrow(Unit*);
	void Move_Arrow(Arrow*);

	void Move_A(Unit*,POINT,
		bool,bool,bool,bool);

	void Move_M(Unit*);

	inline void BoundUp(double, double*, double*, double, double);
	inline void BoundDown(double, double*, double*, double, double);
	
};

