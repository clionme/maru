
// The AI class for multiple unit control. //

#include "StdAfx.h"
#include "macroAI.h"

macroAI::macroAI(Unit * A_in){

	user_unit = A_in;

	miAI = microAI();
}
macroAI::~macroAI(void){}

//////////////////////////////
// Simple_Team_Control : 
//
//  Sets Target for the units in the team
/////////////////////////////
void macroAI::TeamControl_Simple(LGroup *L){
	g = L->g; 
	u = L->u;

	/*
	-------------------------------
	Move to LGroup reset part
	-------------------------------

	// collect statistics and parse the situation
	//count the number of units
	//this counting is very inefficient, but shouldn't be much of a problem. 
	for ( int i = 0 ; i < NUM_UNIT_TYPES; i++ ) {
		num_a[i] = 0; 
		num_e[i] = 0; 
	}
	for ( int i = 0 ; i < MAX_NUM_UNIT ; i++ ) {
		if ( !u[i].active  || u[i].dead )
			continue;
		if ( u[i].team == team ) 
			num_a[u[i].wep.weapon_type]++;
		else
			num_e[u[i].wep.weapon_type]++;
	}
	*/

	////////////////////////////////////
	// allocate target based on chosen plan
	////////////////////////////////////
	SGroup_AdjustTaskSGroup(u,g);
	if ( L->team == 0 ) {
		Order_GuardArcher(u);		
		Order_BraveArcher(u); // archer does not run, unless the target is very near and targetting the archer.
		for ( int j = 0 ; j < L->print_e_num(); j++ )
			Order_GetArcher(u, L->e(j)->u);
		Form_BasicWepSGroup(u,g);
	}
	else {
		Order_GuardArcher(u);
		Form_BasicWepSGroup(u,g);
		Form_TaskForce(u,g, Unit_sword, 3);
		//Form_TaskForce(u,g, Unit_dagger, 3);
		Order_StaySGrouped(u,g,0);
		Order_BraveArcher(u); 
	}
	for ( int j = 0 ; j < L->print_e_num(); j++ )
		Order_GetArcher(u, L->e(j)->u);
	
	for ( int j = 0 ; j < L->print_e_num(); j++ )
		Target_DistBased(u, L->e(j)->u); //Sets target to rest of the units

	//if ( L->team == 1 ) 
	//	Target_SameSGroupTarget(g,4);


	///////////////////////////
	// First groups are for marching-in 
	///////////////////////////
	SGroup_MarchingIn(u,&g[L->team]);

	///////////////
	// Unit level AI setting
	///////////////
	for ( int i = 0; i < MAX_NUM_UNIT; i++ ) {
		if ( !u[i].active || u[i].dead || u[i].userunit) 
			continue;

		if ( u[i].MV_type == MV_to_P_march ) {			
			miAI.march( &u[i] );	
		}
		else {
			switch(u[i].wep.weapon_type) {
			case 5:
			case 1:
				if ( u[i].AI_preset[1] == GACT_GuardArcher ) 
					miAI.guard_Sword(&u[i], u[i].Target );
				else 
					miAI.simple_Sword(&u[i], u[i].Target);	
				break;
			case 4:
				miAI.simple_Dagger(&u[i], u[i].Target);								
				break;
			case 2:
				if ( u[i].AI_preset[1] == GACT_GuardArcher ) 
					miAI.guard_Sword(&u[i], u[i].Target );
				else 
					miAI.simple_Sword(&u[i], u[i].Target);	
				//miAI.simple_Sword(&u[i], u[i].Target);	


				if ( u[i].ACT_type == ACT_dashNslash ){
					//|| dist((*u),(*u->Target)) > 340 ) {
					u[i].ACT_type = ACT_idle;
					//u[i].AI_sequence = 0;
				}
				if ( u[i].ACT_type != ACT_idle && u[i].ACT_type != ACT_guard )
					u[i].ACT_type = ACT_poke;
				break;
			case 3:
				miAI.simple_Bow(&u[i], u[i].Target );
				break;
			}
		}
	}
}

//////////////////////////////
// Target_DistBased : simple Distance based Target setting 
/////////////////////////////
void macroAI::Target_DistBased(Unit *u, Unit *ou){
	double d;

	//Target setting, picks the one which is near by
	for ( int i = 0; i < MAX_NUM_UNIT; i++ ) {
		//u[i].dist_to_closest_enemy = dist(u[i],(*u[i].Target));
		if (  !u[i].active || u[i].dead  
			|| 
			((u[i].AI_preset[1] == GACT_GetArcher) && (!u[i].Target->dead) )
			||
			(u[i].ACT_type != ACT_idle && u[i].ACT_type != ACT_guard )
			){// this is to avoid the rapid change in target
			if ( u[i].Target != NULL && !u[i].dead) {
				if ( u[i].wep.weapon_type != 3 ) 
					u[i].Target->TargetedBy++;
				else
					u[i].Target->TargetedBy_R++;
			}

			u[i].dist_to_closest_enemy = dist(u[i],(*u[i].Target));
			continue;
		}

		//does not changes if current target is near by and facing the current unit
		if (   u[i].Target->Target != &u[i] 
			|| dist ( u[i], (*u[i].Target) ) > 100.0 ) {
			
			// J : enemy unit
			for ( int j = 0; j <= MAX_NUM_UNIT; j++ ) {
				if (ou[j].active && !ou[j].dead 
					&&
					(ou[j].TargetedBy <5 || u[i].AI_preset[1] == GACT_GuardArcher)
					&&
					(    
						 u[i].wep.weapon_type != Unit_bow 
					  || ou[j].wep.weapon_type != Unit_dagger 
					  || dist(u[i], ou[j]) < WEP_BOW_0_RANGE/2 
					)
					&&
					dist( (*u[i].Lcenter), ou[j]) < BATTLE_RANGE 
					//cautious ... boundary condition is implied on microAI.cpp -> Common_PostScript
				){
					d = dist(u[i], ou[j]) ;
					if ( d < u[i].dist_to_closest_enemy  ) {
						u[i].dist_to_closest_enemy = d;
						u[i].Target = &ou[j];
					}
				}
			}
		}
		if ( u[i].wep.weapon_type != 3 )
			u[i].Target->TargetedBy ++;
		else 
			u[i].Target->TargetedBy_R ++;
		//if ( u[i].Target->userunit && u[i].Target->TargetedBy == 0)
		//	u[i].TargetedBy++;
	}
}

//////////////////////////////
// Order_GetArcher : Targets near by archers if they are not targeted
/////////////////////////////
void macroAI::Order_GetArcher(Unit *u, Unit *ou){
	/////////////////
	//Can be used in offensive plan
	/////////////////
	//  Good when the team has more daggers
	//   prioritise enemy archers
	for ( int i = 0 ; i <= MAX_NUM_UNIT; i++ ) { 	//search through enemy archers
		if (   ou[i].active && !ou[i].dead 
			&& ou[i].wep.weapon_type == Unit_bow
			&& ou[i].TargetedBy == 0 
		) {
			for ( int j = 0; j < MAX_NUM_UNIT; j++ ) { //inefficient. maybe have to try group[0] based targetting. 
				if ( u[j].active 
					&& !u[j].dead 
					//&& u[j].Target->wep.weapon_type != Unit_bow 
					&& ( u[j].wep.weapon_type == Unit_dagger )//|| u[j].wep.weapon_type == Unit_sword )
					&& dist( u[j], ou[i]) < 400 ) {

					u[j].Target = &ou[i];
					u[j].Target->TargetedBy++;			
					u[j].AI_preset[1] = GACT_GetArcher;
					break;				
				}
			}
		}
	}
}

//////////////////////////////
// Order_BraveArcher : Archers does not run.
/////////////////////////////
void macroAI::Order_BraveArcher(Unit *u){
	for ( int i = 0 ; i < MAX_NUM_UNIT; i++ ) {
		if (  u[i].active && !u[i].dead
		   && u[i].wep.weapon_type == Unit_bow ) {
			u[i].AI_preset[1] = GACT_HoldPosition;
		}
	}
}

//////////////////////////////
// Order_GuardArcher : Swords now guards near by archers
/////////////////////////////
void macroAI::Order_GuardArcher(Unit *u){
	////////////////////
	//  Guard Allocation
	////////////////////
	//Can be used in a defensive plan
	//  Good when the team has more archers. 
	//   protect the archers
	//release guard connection for dead pairs
	for ( int i = 0 ; i < MAX_NUM_UNIT; i++ ) {
		if ( u[i].T_second != &u[i] ) {
			if (  (u[i].dead && u[i].active)
				||
				   (dist( (*u[i].T_second), u[i] ) > 400 )) {
				u[i].AI_preset[1] = GACT_idle;
				u[i].T_second->AI_preset[1] = GACT_idle;
				u[i].T_second->T_second = u[i].T_second;
				u[i].T_second  = &u[i];
			}
		}
	}

	//allocates unit to guard
	for ( int i = 0 ; i < MAX_NUM_UNIT; i++ ) {
		if ( !u[i].active || u[i].dead 
			//|| (u[i].wep.weapon_type != Unit_sword && u[i].wep.weapon_type != Unit_spear)
			|| (u[i].wep.weapon_type != Unit_spear)
			|| u[i].T_second != &u[i] ) 
			continue;

		for ( int j = 0 ; j < MAX_NUM_UNIT; j++ ) {
			if (   !u[j].dead 
				 && u[j].active 
				 && j != i
				 && dist(u[j],u[i]) < 400
				 && u[j].wep.weapon_type == Unit_bow
				 && u[j].T_second == &u[j]) { //T_second is reseted to itself when the unit is dead or reseted
				u[i].T_second = &u[j];
				u[j].T_second = &u[i];
				u[i].AI_preset[1] = GACT_GuardArcher;
				break;
			}
		}
	}
}

//////////////////////////////
// Order_SameTarget : the group[0]s with given unit type goes for a same target
/////////////////////////////
void macroAI::Target_SameSGroupTarget( SGroup *g, int type){

	for ( int i = tf_group_st ; i < MAX_NUM_SGROUP; i++ ) {
		if (   g[i].isNempty() && g[i].print_num_member() > 1 
			&& g[i].unit_type == type ){

			//find the Closest target
			//use this function after 'GetArcher' 
			// to have this group[0] to target the enemy archer.
			g[i].GTarget = g[i].member[0]->Target;
			g[i].GTarget_dist = g[i].member[0]->dist_to_closest_enemy;
			for ( int j = 1 ; j < g[i].print_num_member(); j++ ) {
				if ( g[i].GTarget_dist > g[i].member[j]->dist_to_closest_enemy ){
					g[i].GTarget = g[i].member[j]->Target;
					g[i].GTarget_dist = g[i].member[j]->dist_to_closest_enemy;
				}
			}

			//assigns the same target to the group[0]
			for ( int j = 0 ; j < g[i].print_num_member(); j++ ){
				g[i].member[j]->Target = g[i].GTarget;
				g[i].member[j]->dist_to_closest_enemy = g[i].GTarget_dist;
			}
		}
	}
}

//////////////////////////////
// Order_StaySGrouped : Stays in group[0] ..
//   mode 0 : for archers
/////////////////////////////
void macroAI::Order_StaySGrouped(Unit *u, SGroup *g, int mode){
	int g_index; 
	/////////////////////
	//Slowly Advances
	//  the whole team stays near each other

	for ( int i = 0 ; i < MAX_NUM_UNIT; i++ ) {
		if ( u[i].group[0] == NOT_IN_SGROUP  )
			continue;
		if ( mode == 0 && u[i].wep.weapon_type != Unit_bow) 
			continue;

		g_index = wep_group(i);
		g[g_index].GTarget_dist = WEP_BOW_0_RANGE; // resets the value

		if ( !g[g_index].isNempty() ) {
			g[g_index].GTarget = u[i].Target;
			g[g_index].GTarget_dist = u[i].dist_to_closest_enemy; // may not be the correct way .. 
		}
		else { //finds the closest target and sets it as the target of the group[0].
			if ( u[i].dist_to_closest_enemy < g[g_index].GTarget_dist ) {
				g[g_index].GTarget = u[i].Target;
				g[g_index].GTarget_dist = u[i].dist_to_closest_enemy; // may not be the correct way .. 
			}
		}

		if ( g[g_index].GTarget_dist < WEP_BOW_0_RANGE ) {
			u[i].AI_preset[1] = GACT_Stay_Close;
			u[i].MV_dest = g[g_index].center;
		}
		else {
			u[i].AI_preset[1] = GACT_idle;
		}
	}
}

//////////////////////////////
// Form_BasicWepSGroup : Forms group[0] of Units with same type of weapons 
/////////////////////////////
void macroAI::Form_BasicWepSGroup(Unit *u, SGroup *g){
	int wg_index; 
	
	for ( int i = 0 ; i <= MAX_NUM_UNIT; i++ ) {
		//makes all wep group[0]s
		if ( u[i].active && !u[i].dead 
			&& u[i].group[0] == NOT_IN_SGROUP ) {

			wg_index = wep_group(i);

			g[wg_index].add(&u[i]);
			g[wg_index].unit_type = u[i].wep.weapon_type;
		}
	}
}

//////////////////////////////
// Form_TaskForce : Forms a group[0] of units with the given weapon type of the given maximum size
/////////////////////////////
void macroAI::Form_TaskForce(Unit *u, SGroup *g, int type, int max_size){
	int wg_index, tg_index;

	//////////////////////
	//forming a new group[0]
	for ( int i = MAX_NUM_SGROUP-1; i > MAX_NUM_TEAM + NUM_UNIT_TYPES * MAX_NUM_TEAM; i -- ) {
		tg_index = i;
		if ( g[i].isNempty() )
			continue;
		else {
			//this group[0] is empty, we use it.		
			break;
		}
	}
	if ( tg_index == MAX_NUM_TEAM + NUM_UNIT_TYPES * MAX_NUM_TEAM ) 
		return; // all group[0] spaces are filled up

	wg_index = MAX_NUM_TEAM + NUM_UNIT_TYPES * u[0].team + type;

	for ( int i = 0; i < MAX_NUM_UNIT; i++ ) {
		if (u[i].active && !u[i].dead && u[i].wep.weapon_type == type 
			&& ( 
					   u[i].group[0] == NOT_IN_SGROUP 
					|| u[i].group[0] == wg_index 
					|| (g[tg_index].isNempty() && g[u[i].group[0]].print_num_member() == 1 && u[i].group[0] >= MAX_NUM_TEAM)
			   )
			&& (!g[tg_index].isNempty() || dist(g[tg_index].center, u[i]) < TF_BOUNDARY ) ){

				g[u[i].group[0]].remove(&u[i]);
				g[tg_index].add(&u[i]);
				if ( g[tg_index].print_num_member() == max_size )
					break;
		}
	}
	
	/////////////////////
	//Into exsisting group[0]
	wg_index = MAX_NUM_TEAM + NUM_UNIT_TYPES * u[0].team + type;

	for ( int i = 0; i < MAX_NUM_UNIT; i++ ) {
		if (u[i].active && !u[i].dead && u[i].wep.weapon_type == type 
			&& ( 
					   u[i].group[0] == NOT_IN_SGROUP 
					|| u[i].group[0] == wg_index 
					|| g[u[i].group[0]].print_num_member() == 1 )  ){

				for ( tg_index = MAX_NUM_SGROUP-1; tg_index > MAX_NUM_TEAM + NUM_UNIT_TYPES * MAX_NUM_TEAM; tg_index-- ) {
					if (   g[tg_index].isNempty() 
						&& dist(g[tg_index].center, u[i]) < TF_BOUNDARY 
						&& g[tg_index].print_num_member() < max_size 
						&& g[tg_index].unit_type == type) {
							if (u[i].group[0] != NOT_IN_SGROUP)
								g[u[i].group[0]].remove(&u[i]);
							g[tg_index].add(&u[i]);
							break;
					}
				}
				if ( tg_index ==  MAX_NUM_TEAM + NUM_UNIT_TYPES * MAX_NUM_TEAM ) 
					return; // all group[0] spaces are filled up
		}
	}
}

//////////////////////////////
// Target_CountReset : Resets the target count
/////////////////////////////
// can get a lot more efficient...
void macroAI::TargetCountReset(LGroup *L, Unit *u){
	
	//reset Targeted counter
	for ( int i = 0; i <= MAX_NUM_UNIT; i++ ) {
		u[i].TargetedBy = 0;
		u[i].TargetedBy_R = 0;

		//Dummy target setting to avoid NULL targets
		if ( u[i].Target == NULL || u[i].Target->dead ) {
			u[i].Target = &L->uDefaultDestination;
		}
		u[i].dist_to_closest_enemy = BATTLE_RANGE;
	}
}

///////////////////
// SGroup control
//    For the group[0]s marchingin after respawn. 
//     1. breaks part of the group[0] gradually when it is close to some opponent units 
//	   2. breaks group[0] when it reaches the destination
///////////////////
inline void macroAI::SGroup_MarchingIn(Unit *u, SGroup *g){
	double d_to_e; //distance to the cloesest enemy from the group[0]
	double d_to_e_threshold = 400.0; 

	d_to_e = d_to_e_threshold  + 1.0; 
	
	if ( g->unit_type == Unit_bow ) 
		d_to_e_threshold *= 2;

	for ( int i = 0 ; i < MAX_NUM_UNIT; i++ ) { 
		if ( !u[i].active || u[i].dead ) 
			continue;
		if ( !g->hasUnit(&u[i]) ) 
			continue;
		if ( u[i].dist_to_closest_enemy < d_to_e ) {
			d_to_e = u[i].dist_to_closest_enemy;
		}
		//if the group[0] has arrived to the target position
		if ( dist( u[i],(u[i].MV_dest)) < 70.0 ) {
			d_to_e = d_to_e_threshold -1.0;
		}

		//if the unit is being attacked, march ends
		//if ( u[i].stat_currentHP < u[i].stat_maxHP )
		//	d_to_e = d_to_e_threshold -1.0;
	}

	if ( d_to_e < d_to_e_threshold ) { //then break the group[0]
		for ( int i = 0 ; i <= MAX_NUM_UNIT; i++ ) {
			if ( !g->hasUnit(&u[i]) ) 
				continue;
			g->remove(&u[i]);
			u[i].MV_type = MV_to_P;
		}
	}
}

//////////////
// Adjust task group[0]s 
//	1. delete group[0] with only one member
//  this one is disabled -> 2. remove member of a group[0] who is too far away from its center
//////////////
inline void macroAI::SGroup_AdjustTaskSGroup(Unit *u, SGroup *g){

	for ( int i = 0; i < MAX_NUM_UNIT; i++ ){
		if (   u[i].active && !u[i].dead 
			&& u[i].group[0] != NOT_IN_SGROUP
			&& u[i].group[0] > ( NUM_UNIT_TYPES + 1 ) * MAX_NUM_TEAM

			&& 
			//(g[u[i].group[0]].print_num_member() == 1 || dist(g[u[i].group[0]].center, u[i]) > TF_BOUNDARY )  ) {
			dist(g[u[i].group[0]].center, u[i]) > TF_BOUNDARY ) {
				g[u[i].group[0]].remove(&u[i]);
				g[wep_group(i)].add(&u[i]);

/*
		){ 
			if ( g[u[i].group[0]].print_num_member() == 1 ) {
				g[u[i].group[0]].remove(&u[i]);
				if ( u[i].team == 0 ) 
					g[25].add(&u[i]);	
				else
					g[24].add(&u[i]);	
			}
			if ( dist(g[u[i].group[0]].center, u[i]) > TF_BOUNDARY  ){
				g[u[i].group[0]].remove(&u[i]);
				if ( u[i].team == 0 ) 
					g[27].add(&u[i]);	
				else
					g[26].add(&u[i]);	

			}
	*/	

		}
	}
}

void macroAI::DeadAnimation( LGroup *L ){
	//Sets AI for the Units. If it is dead, dead sequence is performed
	for ( int i =0 ; i <= MAX_NUM_UNIT; i++ ) {
		if ( L->u[i].active && !L->u[i].userunit) {
			if (L->u[i].dead) {
				L->u[i].DA_counter--;
				if ( L->u[i].DA_counter < 0 )
					L->u[i].active = false;
			}
		}
	}
}