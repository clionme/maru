#include "StdAfx.h"
#include "battleFEE.h"
#include <math.h>


//////////////////////////
// Adjusts the enemy / friendly pointer to close-by LGroups

//   within_battle_range = true means the units are deployed on the battle field. ( it is all done here )
//////////////////////////
void battleFEE::FEE_Adjust_ef(LGroup *L, LGroup * AL){

	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].active ) 
			continue;					
		L[k].calc_center();
	}

	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].active )//|| L[k].usergroup)
			continue;

		//if ( L[k].usergroup ) 
		//	L[k].within_battle_range = true;

		FEE_Update_LG_Status( &L[k], AL );
	}

	// Adjusts ef of LGroups
	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( L[k].active && L[k].within_battle_range && !L[k].empty ) {
			for ( int i = k ; i < MAX_NUM_LGROUP; i++ ) {
				if (L[i].active && L[i].within_battle_range && !L[i].empty) {
					if (L[i].team == L[k].team ) {
						L[k].Sel_add_f(&L[i]);
						L[i].Sel_add_f(&L[k]);
					}
					else {
						L[k].Sel_add_e(&L[i]);
						L[i].Sel_add_e(&L[k]);
					}
				}
			}
		}
	}

	// Sets the group formation and march destination for the newly entered groups 
	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( L[k].active && L[k].within_battle_range 
			&& !L[k].empty 
			//&& L[k].print_e_num() > 0 
			&& L[k].Status == LS_idle ) {

			FEE_Stepin(&L[k]);
			L[k].Status = LS_march;
		}

		if ( L[k].active && !L[k].within_battle_range 
			&& !L[k].empty ) {
			L[k].Status = LS_idle;
		}
		L[k].Set_boundary();

		if ( L[k].active && L[k].print_e_num() == 0 && L[k].g[L[k].team].print_num_member() == 0) {
			if (L[k].usergroup) { 
				L[k].u[MAX_NUM_UNIT].dead = false;
				L[k].u[MAX_NUM_UNIT].stat_currentHP = L[k].u[MAX_NUM_UNIT].stat_maxHP;

				if ( !AL->user_order_given )  {
					for ( int i = 0; i < MAX_NUM_UNIT; i++ ) {
						L[k].u[i].MV_dest.x = L[k].u[MAX_NUM_UNIT].x;
						L[k].u[i].MV_dest.y = L[k].u[MAX_NUM_UNIT].y;
						L[k].u[i].MV_type = MV_to_P;
						L[k].u[i].ACT_type = ACT_idle;
					}
				}
				else {
					AL->user_order_given = false;
					FEE_Stepin(&L[k]);	
					L[k].center.x = L[k].u[MAX_NUM_UNIT].x;
					L[k].center.y = L[k].u[MAX_NUM_UNIT].y;
				}
				//
				//L[k].uDefaultDestination.x = L[k].u[MAX_NUM_UNIT].x;// L[k].center.x;
				//L[k].uDefaultDestination.y = L[k].u[MAX_NUM_UNIT].y;//L[k].center.y;
				//			
			}
			else {
				FEE_Stepin(&L[k]);
			}
		}
	}
}

////////////////////
// manipulates LGroup that has just entered the range of user's LGroup
///////////////////
void battleFEE::FEE_Stepin(LGroup *L){
	
	L->SG_reset();

	for ( int i = 0; i <= MAX_NUM_UNIT; i++ ) {
		if ( L->u[i].active && !L->u[i].dead ) {
			SimpleMeta_MFormMarchSGroup(L, &L->u[i]);  //Need to know how many units are being respawned.
		}
	}

	for ( int i = 0; i <= MAX_NUM_UNIT; i++ ){
		if ( L->u[i].active && !L->u[i].dead ) {

			SimpleMeta_MResetUnit(&L->u[i]);
			L->u[i].group[0] = L->u[i].team;
			L->u[i].MV_type = MV_to_P_march;
			L->u[i].speed = L->g[L->u[i].team].speed;

			//we position the units here.
			cp.x = L->center.x;
			cp.y = L->center.y;

			dp.x = L->uDefaultDestination.x;
			dp.y = L->uDefaultDestination.y;
			
			FEE_Group_Position(&L->g[L->team],&L->u[i],cp,dp,0);
		}
	}
}


////////////////
// Calculates LGroup's status
//   1. distance to user group
//   2. active - or - not (counts the number of units)
//   3. determines whether to include in the user's battle field. (closeness)
////////////////
void battleFEE::FEE_Update_LG_Status(LGroup *L, LGroup* AL){

	if ( L->empty && !L->usergroup)
		L->within_battle_range = false; //usergroup is also ruled out at Maru.cpp 

	for ( int i = 0; i < L->print_e_num(); i++ )
		if ( L->e(i)->empty )
			L->Sel_remove_e(L->e(i));

	for ( int i = 0; i < L->print_f_num(); i++ )
		if ( L->f(i)->empty )
			L->Sel_remove_f(L->f(i));

	//makes the closest enemy group as e(0)
	L->Sel_set_order_e();

	if ( L->print_e_num() != 0 && !L->usergroup ) {

		L->uDefaultDestination.x = L->e(0)->center.x;
		L->uDefaultDestination.y = L->e(0)->center.y;
	}
	
	//updates unit-march destination
	for ( int i = 0; i < MAX_NUM_UNIT; i++ ){
		if (L->u[i].MV_type == MV_to_P_march){
			dp.x = L->uDefaultDestination.x;
			dp.y = L->uDefaultDestination.y;
			cp.x = L->center.x;
			cp.y = L->center.y;
			L->u[i].MV_dest.x = L->u[i].x + dp.x - cp.x ;
			L->u[i].MV_dest.y = L->u[i].y + dp.y - cp.y ;
		}
	}

	if ( dist(L->center, AL->center) < BATTLE_RANGE*0.8 ) { //conditions have to be changed to consider 4 corners
		//Stepping into battle range.
		if ( L->within_battle_range == false ) {
			L->within_battle_range = true;
		}
	}
	else if ( dist(L->center, AL->center) > BATTLE_RANGE*1.2 ) {//conditions have to be changed to consider 4 corners
		//Stepping out from battle range.
		if ( L->within_battle_range == true ) {
			L->within_battle_range = false;
			L->Sel_reset();
		}
	}
};

///////////////////
// Position the units within LGroup
//////////////////
void battleFEE::FEE_Group_Position(SGroup* g, Unit* u, dPOINT cp, dPOINT dp, int type){
	switch(type){
	case 0:

		g->position_remove(u);

		if ( g->position_sequence_of(u->wep.weapon_type) % 2 == 0 ) {
			u->x = cp.x + 50*g->position_sequence_of(u->wep.weapon_type)/2 + 25;
			u->y = cp.y - 50*g->position_sequence_of(u->wep.weapon_type)/2 - 25;
		}
		else {
			u->x = cp.x - 50*g->position_sequence_of(u->wep.weapon_type)/2;	
			u->y = cp.y + 50*g->position_sequence_of(u->wep.weapon_type)/2;
		}

		if ( u->wep.weapon_type % 2 == 0 ) {
			u->x += 10;
			u->y -= 10;
		}
		else {
			u->x -= 10;
			u->y += 10;	
		}
	
		switch(u->wep.weapon_type){
			case Unit_sword:
				break;
			case Unit_spear:
				u->x -= 50;
				u->y -= 50;
				break;
			case Unit_bow:
				u->x -= 100;
				u->y -= 100;
				break;
			case Unit_dagger:
				u->x -= 150;
				u->y -= 150;
				break;
			case Unit_gsword:
				u->x -= 200;
				u->y -= 200;
				break;
		}

		break;
	}

	// changes the direction of the group formation
	// current formation is for the case where 
	//   dp.x > cp.x 
	//   dp.y > cp.y
	// this ends up for 3 additional types.
	if ( dp.x < cp.x ) 
		u->x = cp.x + (cp.x - u->x); 
	if ( dp.y < cp.y ) 
		u->y = cp.y + (cp.y - u->y); 

	u->MV_dest.x = u->x + dp.x - cp.x ;
	u->MV_dest.y = u->y + dp.y - cp.y ;
}

/////////////////////////
//simple respawn for L[]
/////////////////////////
void battleFEE::SimpleMeta_MDead(LGroup *L, int* num_dead){
	//HP check 
	for ( int k = 0; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].within_battle_range )
			continue;
		for ( int i =0; i <= MAX_NUM_UNIT; i++ ) {
			if ( L[k].u[i].hit ) {
				if ( L[k].u[i].status_slow <= 180 && L[k].u[i].wep.Wep_status != Wep_on_guard) 
					L[k].u[i].status_slow += 20;
				if ( L[k].u[i].stat_currentHP <= 0 ) {
			
					L[k].u[i].status_slow = 0;
					L[k].u[i].stat_currentHP = 0;
					L[k].u[i].DA_counter = 50;
					
					L[k].u[i].ACT_type = ACT_idle;
					L[k].u[i].MV_type = MV_death_ani;
					L[k].u[i].wep.Wep_status = Wep_on_idle;

					L[k].u[i].dead = true;
					num_dead[L[k].u[i].team ]++;
					if ( L[k].u[i].group[0] != NOT_IN_SGROUP )
						L[k].g[L[k].u[i].group[0]].remove(&L[k].u[i]);
				}
				L[k].u[i].hit = false;
			}
		}
	}
}

//////////////////
// Resets the unit and respawns 
//////////////////
void battleFEE::SimpleMeta_MResetUnit(Unit* u){
	//Respawn position 
	//  used for debugging
	if ( u->team == 0 ) { 
		u->x = rand()%100 + WINDOW_WIDTH + WINDOW_WIDTH  ; 
		u->y = rand()%100 + WINDOW_HEIGHT+ WINDOW_HEIGHT ;
	}
	else {
		u->x = rand()%100 + WINDOW_WIDTH + WINDOW_WIDTH + 400 ; 
		u->y = rand()%100 + WINDOW_HEIGHT+ WINDOW_HEIGHT + 500 ;
	}
	u->group[0] = NOT_IN_SGROUP;
	u->dist_to_closest_enemy = 100000000000000000000000.0;//-1;//10000000.0; //randomly big number ...
	u->T_second = u;

	u->Target = NULL;
	u->TargetedBy = 0;
	u->TargetedBy_R = 0;

	u->DA_counter = -10;
	u->status_slow = -1;
	u->hit = false;
	u->dead = false;
	u->wep.Wep_status = Wep_on_idle;

	u->wep.target_angle= mPI;
	u->arwp = NULL;

	u->ACT_type = ACT_idle;				
	u->MV_type = 0;

	u->AI_sequence = 0;
	u->AI_timer = 0;
	u->AI_preset[0] = 0;
	u->AI_preset[1] = GACT_idle;
	u->AI_preset[2] = 0;

	u->roll = false;
	u->direction = 0;
	u->sequence = 0;

	u->wep.x = u->x+30;
	u->wep.old.x = u->wep.x;
	u->wep.y = u->y+30;
	u->wep.old.y = u->wep.y;

	u->wep.pulled_p.x = u->x;
	u->wep.pulled_p.y = u->y;

	u->wep.angle_speed = 0.0;
	u->wep.angle_accel = 0.0;

	switch(u->wep.weapon_type){
		case 1:
			u->wep.pulled =0.0;
			break;
		case 2:
			u->wep.pulled = u->wep.WD->length * 0.5;
			break;
		case 3:
			u->wep.pulled =0.0;
			break;
		case 4:
			u->wep.pulled =0.0;
			break;
	}
}

void battleFEE::SimpleMeta_MRespawn(LGroup* L){
	for ( int k = 0; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].within_battle_range )
			continue;
		for ( int i =0; i <= MAX_NUM_UNIT; i++ ) {
			if ( L[k].u[i].active && L[k].u[i].dead && L[k].u[i].DA_counter < 0 ) {
				SimpleMeta_MFormMarchSGroup(&L[k], &L[k].u[i]);  //Need to know how many units are being respawned.
			}
		}
		for ( int i = 0; i <= MAX_NUM_UNIT; i++ ){
			if ( L[k].u[i].active && L[k].u[i].dead && L[k].u[i].DA_counter < 0 ) {
				//simple respawn
				SimpleMeta_MResetUnit(&L[k].u[i]);

				L[k].u[i].dead = false;
				L[k].u[i].stat_currentHP = L[k].u[i].stat_maxHP;

				L[k].u[i].group[0] = L[k].u[i].team;
				L[k].u[i].MV_type = MV_to_P_march;
				L[k].u[i].speed = L[k].g[L[k].u[i].group[0]].speed;

				//we position the units here.
				SimpleMeta_MRespawnSGroupPosition( &L[k].g[L[k].team] ,&L[k].u[i]);
			}
		}
	}
}

//Puts the respawned unit into a group[0], for aesthetics & better AI formulation.
void battleFEE::SimpleMeta_MFormMarchSGroup(LGroup* L,Unit *u){

	u->group[0] = u->team;

	//picks the min speed
	if ( !L->g[u->team].isNempty() )
		L->g[u->team].speed = u->max_speed;
	else if (L->g[u->team].speed > u->max_speed ) 
		L->g[u->team].speed = u->max_speed;

	L->g[u->team].add(u);
	L->g[u->team].position_add(u);
}

//////////////////////////
//  Positioning Functions for SGrouping respawned Units 
//		1. Sets Initial position of respawned Units
//		2. Sets Target Position of respawned Units
//////////////////////////
inline void battleFEE::SimpleMeta_MRespawnSGroupPosition(SGroup* g, Unit* u){

	if ( u->team == 0 ){
		cp.x = 200.0; 
		cp.y = cp.x +  400.0;

		dp.x = cp.x + 4000;
		dp.y = cp.y + 4000;
	}
	else if ( u->team == 1 ){
		cp.x = 3200.0; 
		cp.y = cp.x +  400.0;

		dp.x = cp.x - 4000;
		dp.y = cp.y - 4000;
	}

	FEE_Group_Position(g,u,cp,dp,0);
}