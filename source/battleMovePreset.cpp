#include "StdAfx.h"
#include "battleMovePreset.h"
#include <math.h>


battleMovePreset::battleMovePreset(void)
{}

battleMovePreset::~battleMovePreset(void)
{}

///////////////////////////
// Move to target unit with minimum distance d
///////////////////////////
void battleMovePreset::Mv_DA(Unit *u){
	u->difM.x = 0.0;
	u->difM.y = 0.0;

	u->x += 0.0001;

	if ( u->DA_counter %4 == 0 ) {
		u->direction ++;
		u->direction %= 4;
	}
}

///////////////////////////
// Move to target unit with minimum distance d
///////////////////////////
void battleMovePreset::Mv_to_TargetU_Guard(Unit *u){
	d = dist( (*(u->Target)), (*u->T_second) );

	if (  d/2.0 < u->MV_dist ){
		//u->T_second ======== u ======== u->Target
		//               1/2       1/2
		u->difM.x = u->x - (u->T_second->x + u->Target->x)/2;
		u->difM.y = u->y - (u->T_second->y + u->Target->y)/2;
	}
	else {
		//u->T_second ======== u =================== u->Target
		//            u->MV_dist
		u->difM.x = u->x - (  u->MV_dist * (u->T_second->x - u->Target->x)/d + u->Target->x );
		u->difM.y = u->y - (  u->MV_dist * (u->T_second->y - u->Target->y)/d + u->Target->y );
	}
}

///////////////////////////
// Move to target unit with minimum distance d
///////////////////////////
void battleMovePreset::Mv_to_TargetU(Unit *u, double d){
	if (  dist( (*(u->Target)), (*u) ) > d ){
		u->difM.x = u->x - u->Target->x;
		u->difM.y = u->y - u->Target->y;
	}
	else {
		u->difM.x = 0.0;
		u->difM.y = 0.0;
	}
}

///////////////////////////
// Maintains distance from target unit with 20% margin
//			can simulate the run-away with setting 'd' a large number
///////////////////////////
void battleMovePreset::Mv_dist_TargetU(Unit *u, double d){
	double dummy = dist( (*(u->Target)), (*u) );

	if (  dummy > 1.1*d ){
		u->difM.x = u->x - u->Target->x;
		u->difM.y = u->y - u->Target->y;
	}
	else if (  dummy < 0.9*d ){
		u->difM.x = u->Target->x - u->x ;
		u->difM.y = u->Target->y - u->y;
	}
	else {
		u->difM.x = 0.0;
		u->difM.y = 0.0;
	}
}

///////////////////////////
// Move to target position
///////////////////////////
void battleMovePreset::Mv_to_Position(Unit *u, dPOINT *destination){
	u->difM.x = u->x - destination->x;
	u->difM.y = u->y - destination->y;
}

///////////////////////////
// Circulates around the Position
///////////////////////////
void battleMovePreset::Mv_circulate_Position(Unit *u, double d, bool clock){
	double dummy = dist( ((u->MV_dest)), (*u) );
	//Moves away or towards the target if is too close or far away
	if (  dummy > 1.1*d ){
		u->difM.x = u->x - u->MV_dest.x;
		u->difM.y = u->y - u->MV_dest.y;
	}
	else if (  dummy < 0.4*d ){
		u->difM.x = u->MV_dest.x - u->x ;
		u->difM.y = u->MV_dest.y - u->y;
	}
	else 
	{
		if ( clock ) {
			u->difM.x = -1.0*(u->y - u->MV_dest.y);
			u->difM.y = (u->x - u->MV_dest.x);
		}
		else {
			u->difM.x = (u->y - u->MV_dest.y);
			u->difM.y = -1.0*(u->x - u->MV_dest.x);
		}
	}
}


///////////////////////////
// Move to target unit with diagonal path
///////////////////////////
void battleMovePreset::Mv_diag_TargetU(Unit *u, bool clock){
	// uses angular Rotatoin matrix with 90 degree rotation
	// (x, y) : (-y,x)
	if ( clock ) {
		u->difM.x = ((u->x - u->Target->x) + (u->y - u->Target->y)) / 2.0;
		u->difM.y = ((u->y - u->Target->y) - (u->x - u->Target->x)) / 2.0;
	}
	else {
		u->difM.x = ((u->x - u->Target->x) - (u->y - u->Target->y)) / 2.0;
		u->difM.y = ((u->y - u->Target->y) + (u->x - u->Target->x)) / 2.0;
	}
}

///////////////////////////
// Move to target unit with zigzag path
///////////////////////////
void battleMovePreset::Mv_diagaway_TargetU(Unit *u, bool clock){
	// uses angular Rotatoin matrix with 90 degree rotation
	// (x, y) : (-y,x)
	if ( clock ) {
		u->difM.x = ((u->Target->x - u->x) + (u->Target->y - u->y)) / 2.0;
		u->difM.y = ((u->Target->y - u->y) - (u->Target->x - u->x)) / 2.0;
	}
	else {
		u->difM.x = ((u->Target->x - u->x) - (u->Target->y - u->y)) / 2.0;
		u->difM.y = ((u->Target->y - u->y) + (u->Target->x - u->x)) / 2.0;
	}
}

///////////////////////////
// Circulates around the target unit
///////////////////////////
void battleMovePreset::Mv_circulate_TargetU(Unit *u, double d, bool clock){
	double dummy = dist( (*(u->Target)), (*u) );
	//Moves away or towards the target if is too close or far away
	if (  dummy > 1.1*d ){
		u->difM.x = u->x - u->Target->x;
		u->difM.y = u->y - u->Target->y;
	}
	else if (  dummy < 0.4*d ){
		u->difM.x = u->Target->x - u->x ;
		u->difM.y = u->Target->y - u->y;
	}
	else 
	{
		if ( clock ) {
			u->difM.x = -1.0*(u->y - u->Target->y);
			u->difM.y = (u->x - u->Target->x);
		}
		else {
			u->difM.x = (u->y - u->Target->y);
			u->difM.y = -1.0*(u->x - u->Target->x);
		}
	}
}