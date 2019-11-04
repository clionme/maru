// The AI class for field control ( LGroup level ). //

#include "StdAfx.h"
#include "fieldAI.h"

fieldAI::fieldAI(void){}
fieldAI::~fieldAI(void){}

//////////////////////
// Moves the LGroup which is not within the range of the battle field
//////////////////////
void fieldAI::simple_Move_To_AL(LGroup* L, LGroup* AL){

	double d = dist(AL->center, L->center);

	//L->center.x += ( AL->center.x > L->center.x ) ? 10.0 : ( -10.0);
	//L->center.y += ( AL->center.y > L->center.y ) ? 10.0 : ( -10.0);
	if (!L->within_battle_range ) {
		if ( d< 1500.0)
			return;
		L->center.x += 3.0*( AL->center.x - L->center.x )/d;
		L->center.y += 3.0*( AL->center.y - L->center.y )/d;
		L->uDefaultDestination.x = AL->center.x;
		L->uDefaultDestination.y = AL->center.y;
	}
	else if ( L->print_e_num() == 0 ){
		L->uDefaultDestination.x = AL->center.x;
		L->uDefaultDestination.y = AL->center.y;
	}
}

//////////////////////
// Moves the LGroup which is not within the range of the battle field
//////////////////////
void fieldAI::simple_Move_To_Closest(LGroup* L){

	double d;
	int target_id;

	for ( int k = 0; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].active || L[k].empty || L[k].usergroup )
			continue;
		target_id = -1;
		d  = -1;
		for ( int i = 0; i < MAX_NUM_LGROUP; i++ ) {
			if ( !L[i].active || L[i].empty )
				continue;
			if ( i == k || L[k].team == L[i].team ) 
				continue;
			
			if ( dist(L[k].center, L[i].center) < d || d < 0)  {
				d = dist(L[k].center, L[i].center);
				target_id = i;
			}
		}
		if ( target_id >= 0 )
			simple_Move_To_AL(&L[k], &L[target_id]);
		else //no target left
			simple_Move_To_AL(&L[k], &L[0]);
	}
}


///////////////////////
// Adjusts march destination 
///////////////////////
void fieldAI::LGroup_Adjust_march_destination(LGroup* L){
	/*
	if ( L->print_e_num() == 0 ) {
		L->Status = LS_idle;
		return;
	}
	else {
		L->uDefaultDestination.x = 
		for ( int i = 0 ; i < L->print_e_num(); i++ ) {

		}
	}
	*/
}