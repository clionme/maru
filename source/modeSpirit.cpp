#include "StdAfx.h"
#include "modeSpirit.h"


modeSpirit::modeSpirit(POINT *cursor_in){
	p_cur = cursor_in;
}
modeSpirit::~modeSpirit(void){}

void modeSpirit::reset(){
	mode_on = false;
	charge = 0;
}

//////////////////////////////
//  Main function of spirit mode
//////////////////////////////
void modeSpirit::Spirit_Main(LGroup* L){

	//Gathers dead unit's spirit
	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		//can use empty ( this causes units from emptied-groups to be un-reachable )
		if ( !L[k].active || !L[k].within_battle_range ) 	
			continue;
		for ( int i = 0; i < MAX_NUM_UNIT; i++ ) {
			if ( !L[k].u[i].dead || !L[k].u[i].active )
				continue;
			
			if ( dist(cur, L[k].u[i].wep) < 50 ) {
				L[k].u[i].active = false;//.dead = false;
				charge += 10;
			}
		}
	}

}