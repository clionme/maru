#pragma once

#include "DataTypes.h"
#include "microAI.h"
#include <math.h>

/*

The AI class for team control.

Mainly concerns the actions selected for the units based on 

team vs team scenario

Main concerns on controlling the team.
  - Unit Target setting ( also Aggrresive / Defensive selection ) 
  - Unit Guarding ( for guarding archers ) and formaiton. 

*/

#define TF_BOUNDARY WEP_BOW_0_RANGE

//default weapon groups
#define wep_group(i) MAX_NUM_TEAM + NUM_UNIT_TYPES * (u[i].team) + u[i].wep.weapon_type

//start of tf groups
#define tf_group_st (NUM_UNIT_TYPES+1) * MAX_NUM_TEAM

class macroAI
{
	Unit *user_unit;

	Unit *u;
	SGroup *g;

	//variables for control
	int num_e[NUM_UNIT_TYPES]; //enemy
	int num_a[NUM_UNIT_TYPES]; //ally

	void Target_DistBased(Unit*, Unit*); //use as primary targetting
	void Target_SameSGroupTarget(SGroup *, int); //use as an option 

	void Order_GetArcher(Unit*, Unit*);
	void Order_GuardArcher(Unit*);
	void Order_BraveArcher(Unit*);
	void Order_StaySGrouped(Unit *, SGroup *,int);

	void Form_BasicWepSGroup(Unit *, SGroup *);
	void Form_TaskForce(Unit *, SGroup *, int, int);

	inline void SGroup_MarchingIn(Unit *, SGroup *);

	inline void SGroup_AdjustTaskSGroup(Unit *, SGroup *);
	

	microAI miAI;

public:
	macroAI( Unit *);
	macroAI(void){};
	~macroAI(void);

	//void TeamControl_Simple(Unit *, SGroup *g, int );
	void TargetCountReset(LGroup*,Unit*);
	void TeamControl_Simple( LGroup *L );
	void DeadAnimation( LGroup *L );
	
};