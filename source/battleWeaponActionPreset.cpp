#include "StdAfx.h"
#include "battleWeaponActionPreset.h"
#include <math.h>

battleWeaponActionPreset::battleWeaponActionPreset(void){}
battleWeaponActionPreset::~battleWeaponActionPreset(void){}

double battleWeaponActionPreset::angle_between(double x1,double y1, double x2, double y2){
	//origin : (x1, y1)

	/*          (x2,y2)
			   /
			  /
			 /
			/
	(x1,y1) ----------
	*/

	double angle;

	angle = atan((y2 - y1) / (x2 - x1));

	if ( x2 - x1 < 0  )
		angle += mPI;
	else if ( y2 - y1 < 0  )
		angle += mPI*2;

	return angle;
}

//////////////////
// Random swing
// Swings the weapon towards the target
/////////////////
 void battleWeaponActionPreset::Atk_Swing(Unit* u){

	double d;

	if ( u->wep.pulled > 0 )
		u->wep.pulled -= u->wep.WD->length* u->wep.WD->pull_speed;

	//stores current weapon position
	u->wep.old.x = u->wep.x;
	u->wep.old.y = u->wep.y;

	//initializes the swing sequence
	if ( !(u->wep.Wep_status == Wep_on_swing) ){//&& u->wep.angle_speed < 0.05 ) {
		u->wep.Wep_status = Wep_on_swing; //the weapon is now on swing sequence
		u->wep.angle_accel = 0.010;
		//angle between u' weapon and u
		u->wep.angle 
			= angle_between(u->x, u->y, 
							u->wep.x, u->wep.y);

		u->wep.target_angle 
			= angle_between(u->x, u->y, 
							u->Target->x, u->Target->y);

		//sets target angle
		if ( u->wep.target_angle  > u->wep.angle ) {					
			d = u->wep.target_angle - u->wep.angle ;
			if ( d > mPI) {
				u->wep.direction = false; //c-clock wise
				u->wep.target_angle = u->wep.angle - 4.0*mPI ;
			}
			else {
				u->wep.direction = true; //clock wise
				u->wep.target_angle = u->wep.angle + 4.0*mPI ;
			}
		}
		else { 
			d = u->wep.angle - u->wep.target_angle ;								
				
			if ( d < mPI) {
				u->wep.direction = false; //c-clock wise
				u->wep.target_angle = u->wep.angle - 4.0*mPI ;
			}
			else {
				u->wep.direction = true; //clock wise
				u->wep.target_angle = u->wep.angle + 4.0*mPI ;
			}
		}	
	}
	else {
		//weapon is on swing sequence
		//checks whether the target angle is reached or not

		// increases the degree between the weapon and u[i] to perform a swing
		if (u->wep.direction) {
			if ( u->wep.target_angle - u->wep.angle > 0.0 ) {
				
			}
			else //end of swing
			{	CALL_uAI;
				u->wep.Wep_status = Wep_on_idle;
				u->wep.angle_accel = 0.00;
			}
		}
		else {
			if ( u->wep.target_angle - u->wep.angle < 0.0 ) {

			}
			else //end of swing
			{	
				CALL_uAI;
				u->wep.Wep_status = Wep_on_idle;
				u->wep.angle_accel = 0.00;
			}
		}
	}							

	MoveWep(u);
	//BoundWepSpeed(u);//stick with angle speed max.
	BoundWep(u);
}

 //////////////////
// Half swing
// Swings the weapon towards the target, 270 degree swing
/////////////////
 void battleWeaponActionPreset::Atk_HSwing(Unit* u){

	 //first makes the angle between the target and the weapon greater than 90degrees
	 //then swings the weapon towards the target.
	 //swing ends when the weapon is 45degree away from the target

	double d, d1, d2;
	double swing_accel = 0.005 * 4.0;

	//stores current weapon position
	u->wep.old.x = u->wep.x;
	u->wep.old.y = u->wep.y;

	PULL_out; //pushes out the pulled weapon

	//initializes the swing sequence
	if ( !(u->wep.Wep_status == Wep_on_swing) ){//&& u->wep.angle_speed < 0.10 ) {	

	//	PULL_out; //pushes out the pulled weapon

		//angle between u' weapon and u
		u->wep.angle 
			= angle_between(u->x, u->y, 
							u->wep.x, u->wep.y);

		u->wep.target_angle 
			= angle_between(u->x, u->y, 
							u->Target->x, u->Target->y);

		d = fabs(u->wep.target_angle - u->wep.angle) ;

		if ( d > mPI ) 
			d = 2.0*mPI - d;

		// checks which direction the swing should take.
		// goes to the direction which is closer to the line which is tangent to the target-origin.
		/*

        d2 : angle between tangent line vs weapon, in counter clock direction
		d1 : angle between tangent line vs weapon, in clock direction

		//////////////////////
		//for positioning phase
		//////////////////////

					   target  weapon
						   |  /
					d2     | /   d1  
						   |/      
		tangent line ------o-------------

			
			if ( d1 > d2 ) 
					go clock direction
			if ( d1 < d2 )
					go counter clock direction


		//////////////////////
		//for swing phase
		//////////////////////

					   target 
						   |  
					       |    
						   |      
		tangent line ------o-------------
						  /
					 d2  /   d1
						/
					   /
					weapon

			if ( d1 > d2 ) 
					go counter clock direction
			if ( d1 < d2 )
					go clock direction


		*/
		d1 = u->wep.target_angle +mPI / 2.0;
		if ( d1 > 2*mPI )
			d1 -= 2*mPI;
		d1 = fabs(d1 - u->wep.angle );
		if ( d1 > mPI ) 
			d1 = 2.0*mPI - d1;

		d2 = u->wep.target_angle -mPI / 2.0;
		if ( d2 < 0 ) 
			d2 += 2*mPI;
		d2 = fabs(d2 - u->wep.angle );
		if ( d2 > mPI ) 
			d2 = 2.0*mPI - d2;

		/////////////////////////
		// Swing phase
		if ( d > mPI / 2.0 ) {

			if ( d1 < d2 ) {
				u->wep.direction = false; //c-clock wise
				u->wep.target_angle = u->wep.angle - d1 - mPI/ 2.0;
			}
			else {
				u->wep.direction = true; //clock wise
				u->wep.target_angle = u->wep.angle + d2 + mPI/ 2.0;
			}

			//the weapon is now on swing sequence
			u->wep.Wep_status = Wep_on_swing;
			u->wep.angle_accel = swing_accel;
		}

		///////////////////////
		// Positioning phase		
		else {
			u->wep.Wep_status = Wep_on_idle; //the weapon is on positioning sequence
			u->wep.angle_accel = swing_accel*2.0;
		
			//sets target angle
			if ( d1 > d2 ) {
				/* angle is measured counter clockwise
				t ------u        
						|
					d	|
						w
				*/
				u->wep.target_angle -= mPI / 2.0;
				u->wep.direction = false; //c-clock wise
			}
			else {
				/* angle is measured counter clockwise
				w ------u        
						|
					d	|
						t
				*/

				u->wep.target_angle += mPI / 2.0;
				u->wep.direction = true; //clock wise
			}
		}
	}
	else {
		//weapon is on swing sequence
		//checks whether the target angle is reached or not

		// increases the degree between the weapon and u[i] to perform a swing
		if (u->wep.direction) {
			if ( u->wep.target_angle - u->wep.angle > 0 ){
			//	PULL_out; //pushes out the pulled weapon
			}
			else if ( u->wep.angle_speed > 0.0 ) {
				u->wep.angle_accel = swing_accel * (-0.53);

				//if ( !(u->wep.Wep_status == Wep_on_poke) && u->wep.pulled < u->wep.WD->length *  u->wep.WD->pull_max *1.3) 
				//	u->wep.pulled += u->wep.WD->length* u->wep.WD->pull_speed;//0.2;
			}
			else //end of hswing
			{	
				u->wep.angle_accel = 0.0;
				CALL_uAI;
				u->wep.Wep_status = Wep_on_idle;
			}
		}
		else {
			if ( u->wep.target_angle - u->wep.angle < 0 ) {
			//	PULL_out; //pushes out the pulled weapon
			}
			else if ( u->wep.angle_speed < 0.0 ) {
				u->wep.angle_accel = swing_accel * (-0.53);

				//if ( !(u->wep.Wep_status == Wep_on_poke) && u->wep.pulled < u->wep.WD->length *  u->wep.WD->pull_max *1.3) 
				//	u->wep.pulled += u->wep.WD->length* u->wep.WD->pull_speed;//0.2;
			}
			else //end of hswing
			{	
				u->wep.angle_accel = 0.0;
				CALL_uAI;
				u->wep.Wep_status = Wep_on_idle;
			}
		}
	}		
	
	if ( (u->wep.Wep_status == Wep_on_swing) ) {
		d = dist( (*u), (*u->Target) ); 
		if ( d!= 0 ) {
			u->x += 3.0*( (u->Target->x) - u->x ) / d ;
			u->y += 3.0*( (u->Target->y) - u->y ) / d ;
		}
	}
	
	MoveWep(u);
	//BoundWepSpeed(u);//stick with angle speed max.
	BoundWep(u);
}


 //////////////////
// Full swing
// Swings the weapon towards the target, 360 degree
/////////////////
 void battleWeaponActionPreset::Atk_FSwing(Unit* u){

	 //first makes the angle between the target and the weapon less than 90degrees
	 //then swings the weapon away from the target, hitting the target after traveling 270degrees
	 //swing ends when the weapon is 90 degree away from the target after it hits the target

	double d, d1, d2;
	double swing_accel = 0.005 * 1.5;

	if ( u->wep.pulled > 0 ) //pushes out the pulled weapon
		u->wep.pulled -= u->wep.WD->length* u->wep.WD->pull_speed;

	//stores current weapon position
	u->wep.old.x = u->wep.x;
	u->wep.old.y = u->wep.y;

	//initializes the swing sequence
	if ( !(u->wep.Wep_status == Wep_on_swing) ){
		//angle between u' weapon and u
		u->wep.angle 
			= angle_between(u->x, u->y, 
							u->wep.x, u->wep.y);

		u->wep.target_angle 
			= angle_between(u->x, u->y, 
							u->Target->x, u->Target->y);

		d = fabs(u->wep.target_angle - u->wep.angle) ;

		if ( d > mPI ) 
			d = 2.0*mPI - d;

		// checks which direction the swing should take.
		// goes to the direction which is closer to the line which is tangent to the target-origin.
		/*

        d2 : angle between tangent line vs weapon, in counter clock direction
		d1 : angle between tangent line vs weapon, in clock direction

		//////////////////////
		//for positioning phase
		//////////////////////

					   target  weapon
						   |  /
					d2     | /   d1  
						   |/      
		tangent line ------o-------------

			
			if ( d1 > d2 ) 
					go clock direction
			if ( d1 < d2 )
					go counter clock direction


		//////////////////////
		//for swing phase
		//////////////////////

					   target 
						   |  
					       |    
						   |      
		tangent line ------o-------------
						  /
					 d2  /   d1
						/
					   /
					weapon

			if ( d1 > d2 ) 
					go counter clock direction
			if ( d1 < d2 )
					go clock direction


		*/
		d1 = u->wep.target_angle +mPI / 2.0;
		if ( d1 > 2*mPI )
			d1 -= 2*mPI;
		d1 = fabs(d1 - u->wep.angle );
		if ( d1 > mPI ) 
			d1 = 2.0*mPI - d1;

		d2 = u->wep.target_angle -mPI / 2.0;
		if ( d2 < 0 ) 
			d2 += 2*mPI;
		d2 = fabs(d2 - u->wep.angle );
		if ( d2 > mPI ) 
			d2 = 2.0*mPI - d2;

		/////////////////////////
		// Swing phase
		if ( d < mPI / 2.0 ) {

			if ( d1 < d2 ) {
				u->wep.direction = true; //clock wise
				u->wep.target_angle = u->wep.angle + d1 + 3.0*mPI/ 2.0;
			}
			else {
				u->wep.direction = false; //c-clock wise
				u->wep.target_angle = u->wep.angle - d2 - 3.0*mPI/ 2.0;
			}

			//the weapon is now on swing sequence
			u->wep.Wep_status = Wep_on_swing;  
			u->wep.angle_accel = swing_accel;

		}

		///////////////////////
		// Positioning phase		
		else {
			u->wep.Wep_status = Wep_on_idle; //the weapon is on positioning sequence
			u->wep.angle_accel = swing_accel*6.0;
		
			//sets target angle
			if ( d1 > d2 ) {
				/* angle is measured counter clockwise
				t ------u        
						|
					d	|
						w
				*/
				u->wep.target_angle -= mPI / 2.0;
				u->wep.direction = true; //clock wise
			}
			else {
				/* angle is measured counter clockwise
				w ------u        
						|
					d	|
						t
				*/

				u->wep.target_angle += mPI / 2.0;
				u->wep.direction = false; //c-clock wise
			}
		}
	}
	else {
		//weapon is on swing sequence
		//checks whether the target angle is reached or not

		// increases the degree between the weapon and u[i] to perform a swing
		if (u->wep.direction) {
			if ( u->wep.target_angle - u->wep.angle > 0 ){//-mPI/2.0 ) {
				//if ( u->wep.angle_speed < 0.15 )
				//	u->wep.angle_speed += u->wep.angle_accel;
			}
			else if ( u->wep.angle_speed > 0.0 ) {
				u->wep.angle_accel = swing_accel * (-2.5);
			}
			else //end of hswing
			{	
				u->wep.angle_accel = 0.0;
				CALL_uAI;
				u->wep.Wep_status = Wep_on_idle;
			}
		}
		else {
			if ( u->wep.target_angle - u->wep.angle < 0 ){//mPI/2.0  ) {
				//if ( u->wep.angle_speed > -0.15 )
				//	u->wep.angle_speed -= u->wep.angle_accel;
			}
			else if ( u->wep.angle_speed< 0.0 ) {
				u->wep.angle_accel = swing_accel * (-2.5);
			}
			else //end of hswing
			{	
				u->wep.angle_accel = 0.0;
				CALL_uAI;
				u->wep.Wep_status = Wep_on_idle;
			}
		}
	}							
	
	d = dist( (*u), (*u->Target) ); 
	if (d!= 0) {
		if ( (u->wep.Wep_status == Wep_on_swing && u->wep.Wep_breaks_guard) ) {
			u->x += 7.0*( (u->Target->x) - u->x ) / d ;
			u->y += 7.0*( (u->Target->y) - u->y ) / d ;
		}
		else {
			u->x -= 2.0*( (u->Target->x) - u->x ) / d ;
			u->y -= 2.0*( (u->Target->y) - u->y ) / d ;
		}
	}
	
	MoveWep(u);
	//BoundWepSpeed(u);//stick with angle speed max.
	BoundWep(u);
}


 //////////////////
// Dash and Slash
//   for dagger and sword. Dashes and slashes the target
/////////////////
void battleWeaponActionPreset::Atk_DashNSlash(Unit* u){
	//stores current weapon position
	u->wep.old.x = u->wep.x;
	u->wep.old.y = u->wep.y;

	if ( u->wep.Wep_status != Wep_on_dns ) {
		u->wep.Wep_status = Wep_on_dns;

		u->wep.stance_shift = true;
		u->wep.stance_charge = 0;
		u->wep.next_stance = 1;
	}
	else if ( u->wep.stance_shift ){

		//stance shifter 
		if ( u->wep.stance_charge > 100 ) {
			u->wep.stance_shift = false;
			//u->wep.stance_charge = 0;
			u->wep.stance = u->wep.next_stance;
		}
		else {
			if ( u->wep.weapon_type == 4 ) {
				u->wep.stance_charge += 3;
				// for stance shift animation presets the next stance weapon position
				Rdy_TanAnglePos(u, true, false, 2);
			}
			else if ( u->wep.weapon_type == 1 ) {
				u->wep.stance_charge += 2;
				// for stance shift animation presets the next stance weapon position
				Rdy_TanAnglePos(u, true, false, 3); //slower for sword
			}
			else if ( u->wep.weapon_type == 5 ) {
				u->wep.stance_charge += 1;
				// for stance shift animation presets the next stance weapon position
				Rdy_TanAnglePos(u, true, false, 2); //slower for sword
			}
		}

		//steps backward
		d = dist( (*u), (*u->Target) ); 
		if ( u->wep.weapon_type == 4 ) {
			PULL_in; //pulls weapon in
			u->x -= 1*( (u->Target->x) - u->x ) / d ;
			u->y -= 1*( (u->Target->y) - u->y ) / d ;
			len = 200.0;
		}
		else if ( u->wep.weapon_type == 1 ) {
			PULL_out;
			u->x += (rand()%10-4)*( (u->Target->x) - u->x ) / d ;
			u->y += (rand()%10-4)*( (u->Target->y) - u->y ) / d ;
			len = 400.0;
		}
		else if (u->wep.weapon_type == 5){
			PULL_in;
			u->x += (rand()%20-10)*( (u->Target->x) - u->x ) / d ;
			u->y += (rand()%20-10)*( (u->Target->y) - u->y ) / d ;
			len = 600.0;
		}

		//formulates the target location for dash n slash move towards the target
		//  aims right next to the target with respect to the direction chosen in movement param. 
		//  'u->MV_clock_wise' is controled randomly and changes when the unit hits the boundary of the map.
		if ( u->MV_clock_wise )  {
			u->skill.x = u->Target->x - 30*(u->Target->y - u->y)/d;
			u->skill.y = u->Target->y + 30*(u->Target->x - u->x)/d;

			d = dist( (*u),u->skill );
			u->skill.x = len*(u->skill.x - u->x)/d + u->x;
			u->skill.y = len*(u->skill.y - u->y)/d + u->y;
		}
		else {
			u->skill.x = u->Target->x + 30*(u->Target->y - u->y)/d;
			u->skill.y = u->Target->y - 30*(u->Target->x - u->x)/d;

			d = dist( (*u),u->skill );
			u->skill.x = len*(u->skill.x - u->x)/d + u->x;
			u->skill.y = len*(u->skill.y - u->y)/d + u->y;
		}
	}
	else {
		PULL_out;

		if ( dist( u->skill, (*u) ) > 40 ){
			//performs Dash n slash
			//moves to skill position 
			
			u->x = u->x + ( u->skill.x - u->x )/ (u->wep.weapon_type == 5 ? 40.0 : 20.0);
			u->y = u->y + ( u->skill.y - u->y )/ (u->wep.weapon_type == 5 ? 40.0 : 20.0);
		}

		if ( dist( u->old, (*u) ) < 4  //collision to wall or unit
			) {
			CALL_uAI; //makes the action not to repeat again
			u->wep.Wep_status = Wep_on_idle;
			u->wep.stance = 0;
		}

		switch(u->wep.weapon_type) {
		case 1:
			u->wep.angle_speed = 0.15;
			break;
		case 4:
			u->wep.angle_speed = 0.0;
			break;
		case 5:
			u->wep.x = u->skill.x ;
			u->wep.y = u->skill.y ;
			u->wep.angle_speed = 0.0;//100;
			break;
		}

		if ( u->MV_clock_wise )  {
			u->wep.direction = true;
		}
		else {
			u->wep.direction = false;
			u->wep.angle_speed *= -1.0;
		}
	}

	MoveWep(u);
	//BoundWepSpeed(u);//stick with angle speed max.
	BoundWep(u);
}


 //////////////////
// Spin
// Spins the weapon 
/////////////////
 void battleWeaponActionPreset::Atk_Spin(Unit* u){

	 //Spin !
//	double d, d1, d2;
	 /*
	double swing_accel = 0.05 * 1.5;

	//stores current weapon position
	u->wep.old.x = u->wep.x;
	u->wep.old.y = u->wep.y;

	//angle between u' weapon and u
		u->wep.angle 
			= angle_between(u->x, u->y, 
							u->wep.x, u->wep.y);
	
	u->wep.angle_accel = swing_accel;

	MoveWep(u);
	//BoundWepSpeed(u);//stick with angle speed max.
	BoundWep(u);
	*/
}

//////////////////
// Pulls the Weapon and ready for next thrust / poke
//////////////////
void battleWeaponActionPreset::Rdy_Pull(Unit * u){
	//stores current weapon position
	u->wep.old.x = u->wep.x;
	u->wep.old.y = u->wep.y;

	if ( u->wep.pulled < u->wep.WD->length *  u->wep.WD->pull_max ) 
		u->wep.pulled += u->wep.WD->length *  u->wep.WD->pull_speed;//0.2;
	u->wep.Wep_status = Wep_on_idle;
	u->wep.x = u->Target->x;
	u->wep.y = u->Target->y;

	MoveWep(u);
	BoundWepSpeed(u);
	BoundWep(u);
}

void battleWeaponActionPreset::Rdy_Idle(Unit *u) {
	//if ( u->wep.pulled > 0 )
	if ( u->wep.weapon_type != 2 && u->wep.pulled > 0 )
		u->wep.pulled -= u->wep.WD->length* u->wep.WD->pull_speed;
	else if ( u->wep.weapon_type == 2 ) {
		PULL_in;
	}

	MoveWep(u);
	BoundWepSpeed(u);
	BoundWep(u);
}

void battleWeaponActionPreset::Rdy_Idle_UserUnit(Unit *u, bool LButtonDown, bool RButtonDown) {

	Rdy_TanAnglePos(u, LButtonDown, RButtonDown, 1);
	//Rdy_Idle(u);
	MoveWep(u);
	BoundWepSpeed(u);
	BoundWep(u);
}

//////////////////
// Thrusts / Pokes the weapon
//////////////////
void battleWeaponActionPreset::Atk_Poke(Unit * u){
	double dummy_a, dummy_b, dummy_c;

	//stores current weapon position
	u->wep.old.x = u->wep.x;
	u->wep.old.y = u->wep.y;

	if ( !(u->wep.Wep_status == Wep_on_poke) && u->wep.pulled < u->wep.WD->length *  u->wep.WD->pull_max *1.3) {
		u->wep.pulled += u->wep.WD->length* u->wep.WD->pull_speed;//0.2;
		
		//aims for the center of body
		u->wep.x = u->Target->x - unit_position_adj_x;
		u->wep.y = u->Target->y - unit_position_adj_y;
	}
	else {
		/*
		if ( u->wep.pulled > 0.0 ) { // u->wep.WD->length ) 
			u->wep.pulled -= u->wep.WD->length* u->wep.WD->poke_speed;//0.2;
			u->x += u->wep.WD->poke_move_distance*( u->wep.x - u->x ) / u->wep.WD->length;
			u->y += u->wep.WD->poke_move_distance*( u->wep.y - u->y ) / u->wep.WD->length;
			u->wep.Wep_status = Wep_on_poke;
		}
		else {
			u->wep.Wep_status = Wep_on_idle;
			CALL_uAI;
		}
		*/

		if ( u->wep.pulled > 0.0 ) { // u->wep.WD->length ) 
			if ( u->wep.Wep_status == Wep_on_poke ){
				u->wep.pulled -= u->wep.WD->length* u->wep.WD->poke_speed;//0.2;
				d = dist( (*u), u->wep);
				u->x += u->wep.WD->poke_move_distance*(u->wep.x - u->x) /d;// u->wep.WD->length;
				u->y += u->wep.WD->poke_move_distance*( u->wep.y - u->y ) /d;// u->wep.WD->length;
			}
			else {
				dummy_a = u->y - (u->Target->y - unit_position_adj_y);
				dummy_b = (u->Target->x- unit_position_adj_x) - u->x;
				dummy_c = (u->x*(dummy_a) + u->y*(dummy_b))*(-1.0); 

				if ( u->wep.weapon_type == 4 ) 
					u->wep.Wep_status = Wep_on_poke;

				else if ( (dist( (u->wep), (*u->Target)) < dist( (*u), (*u->Target) )) 
					  && 
					 (fabs(dummy_a * u->wep.x + dummy_b * u->wep.y + dummy_c) / sqrt(pow(dummy_a,2)+pow(dummy_b,2)) < 5.0 ) ){ 
					//distance between line(unit to target) and a point(weapon coordinate) )
					u->wep.Wep_status = Wep_on_poke;
				}
				else {
					//if (dist((*u), (*u->Target)) > u->wep.WD->length ) {
					//	u->wep.Wep_status = Wep_on_idle;
					//	CALL_uAI;
					//}
					u->wep.x = u->Target->x - unit_position_adj_x;
					u->wep.y = u->Target->y - unit_position_adj_y;
				}
			}
		}
		else {
			u->wep.Wep_status = Wep_on_idle;
			CALL_uAI;
		}
	}	

	MoveWep(u);
	BoundWepSpeed(u);
	BoundWep(u);
}

////////////////////////
// Moves weapon to tangential angle against Target unit
////////////////////////
void battleWeaponActionPreset::Rdy_TanAnglePos(Unit * u, bool LButtonDown, bool RButtonDown, int mode){
	
	double d, d1, d2;
	double swing_accel = 0.005 * 1.5;

	//if ( u->wep.pulled > 0 ) //pushes out the pulled weapon
	//	u->wep.pulled -= u->wep.WD->length* u->wep.WD->pull_speed;

	//stores current weapon position
	u->wep.old.x = u->wep.x;
	u->wep.old.y = u->wep.y;

	//angle between u' weapon and u
	u->wep.angle 
		= angle_between(u->x, u->y, 
						u->wep.x, u->wep.y);

	if ( mode != 2 )
		// F,H swing
		u->wep.target_angle 
			= angle_between(u->x, u->y, 
							u->Target->x, u->Target->y);
	else 
		// Dash n Slash
		u->wep.target_angle 
			= angle_between(u->x, u->y, 
							u->skill.x, u->skill.y);

	d = fabs(u->wep.target_angle - u->wep.angle) ;

	if ( d > mPI ) 
		d = 2.0*mPI - d;

	// checks which direction the swing should take.
	// goes to the direction which is closer to the line which is tangent to the target-origin.
	/*

    d2 : angle between tangent line vs weapon, in counter clock direction
	d1 : angle between tangent line vs weapon, in clock direction

	//////////////////////
	//for positioning phase
	//////////////////////

					target  weapon
						|  /
			    	d2  | /   d1  
						|/      
	tangent line ------o-------------

			
		if ( d1 > d2 ) 
				go clock direction
		if ( d1 < d2 )
				go counter clock direction


	*/
	d1 = u->wep.target_angle +mPI / 2.0;
	if ( d1 > 2*mPI )
		d1 -= 2*mPI;
	d1 = fabs(d1 - u->wep.angle );
	if ( d1 > mPI ) 
		d1 = 2.0*mPI - d1;

	d2 = u->wep.target_angle -mPI / 2.0;
	if ( d2 < 0 ) 
		d2 += 2*mPI;
	d2 = fabs(d2 - u->wep.angle );
	if ( d2 > mPI ) 
		d2 = 2.0*mPI - d2;

	///////////////////////
	// Positioning 	
	if ( mode == 1 ) {
		//For Fswing, Hswing
		if ( LButtonDown && d < mPI / 2.0 ){
			u->wep.angle_accel = swing_accel*3.0;
		
			//sets target angle
			if (  (d1 < d2)  ) {
				u->wep.direction = true; //clock wise
			}
			else {
				u->wep.direction = false; //c-clock wise
			}		
		}
		else  if ( RButtonDown && d > mPI / 2.0 ){
			u->wep.angle_accel = swing_accel*d*d*2.0;
		
			//sets target angle
			if (  (d1 < d2)  ) {
				u->wep.direction = false;
			}
			else {
				u->wep.direction = true;
			}		
		}
	}
	
	else if ( mode == 2 ) {
		//for Dash mode - dagger
		if ( d < mPI*15/16 ){		
			//sets target angle
			if (  (d1 > d2)  ) {
				u->wep.angle_speed = 0.05;
				u->wep.direction = true; //clock wise
			}
			else {
				u->wep.angle_speed = -0.05;
				u->wep.direction = false; //c-clock wise
			}		
		}
		else 
			u->wep.angle_speed = 0.0;
	}	
	else if ( mode == 3 ) {
		//for Dash mode - sword
		if ( d < mPI*15/16 ){		
			//sets target angle
			if (  (d1 < d2)  ) {
				u->wep.angle_speed = 0.05;
				u->wep.direction = true; //clock wise
			}
			else {
				u->wep.angle_speed = -0.05;
				u->wep.direction = false; //c-clock wise
			}		
		}
		else 
			u->wep.angle_speed = 0.0;
	}
}


////////////////////////////
// Guard Stance
///////////////////////////
void battleWeaponActionPreset::Rdy_Guard(Unit * u) {
	//basically starts from the pulled status.
	//   rest is up to collision detection
	//stores current weapon position
	u->wep.old.x = u->wep.x;
	u->wep.old.y = u->wep.y;

	if ( u->wep.pulled < u->wep.WD->length *  u->wep.WD->pull_max ) 
		u->wep.pulled += u->wep.WD->length *  u->wep.WD->pull_speed*2.0;//0.2;

	if ( u->wep.pulled > 0.5* u->wep.WD->length *  u->wep.WD->pull_max ) 
		u->wep.Wep_status = Wep_on_guard;
	else
		u->wep.Wep_status = Wep_on_idle;

	u->wep.x = u->Target->wep.c[1].x ;//- 30;
	u->wep.y = u->Target->wep.c[1].y ;//+ 30;
	MoveWep(u);
	BoundWepSpeed(u);
	BoundWep(u);
}

///////////////////////
// Bounds Weapon within in its length, also calculates pulled dist.
///////////////////////
inline void battleWeaponActionPreset::BoundWep(Unit* u){
	
	d = dist(u->wep, (*u) );
	len = u->wep.WD->length - u->wep.pulled;

	if ( d > len ) {
		u->wep.x = u->x + len*(u->wep.x-u->x) / d;
		u->wep.y = u->y + len*(u->wep.y-u->y) / d;
	}
	/*
	else if ( d < len ) {
		u->wep.x = u->x + len*(u->wep.x-u->x) / d;
		u->wep.y = u->y + len*(u->wep.y-u->y) / d;
	}	
	*/
	u->wep.pulled_p.x = u->x 
		- (u->wep.x - u->x)*(u->wep.pulled/(u->wep.WD->length-u->wep.pulled));
	u->wep.pulled_p.y = u->y 
		- (u->wep.y - u->y)*(u->wep.pulled/(u->wep.WD->length-u->wep.pulled));
}


///////////////////////
// Bounds Weapon Speed based on its old position
///////////////////////
inline void battleWeaponActionPreset::BoundWepSpeed(Unit* u){
	
	d = dist( u->wep, u->wep.old );
	len = u->wep.WD->max_plane_speed;

	if ( d > len ) {
		u->wep.x = u->wep.old.x + len*(u->wep.x - u->wep.old.x) / d;
		u->wep.y = u->wep.old.y + len*(u->wep.y - u->wep.old.y) / d;
	}
}

//////////////////
// Actually moves the weapon. 
//////////////////
 inline void battleWeaponActionPreset::MoveWep(Unit * u){

	if ( u->wep.Wep_status == Wep_on_dns )  {
		//uses its own speed setting
	}
	else {
		if ( !(u->wep.Wep_status == Wep_on_swing) ){
			u->wep.angle_speed *= 0.7;
			u->wep.angle_accel *= 0.9;//-1.0*u->wep.angle_speed*0.1;
		}

		if ( fabs(u->wep.angle_speed) < 0.3 ) {
			if (u->wep.direction)
				u->wep.angle_speed += u->wep.angle_accel;
			else
				u->wep.angle_speed -= u->wep.angle_accel;
		}
		else {
			u->wep.angle_speed /= fabs(u->wep.angle_speed);
			u->wep.angle_speed *= 0.2999;
		}
	}

	if ( (fabs(u->wep.angle_speed) > 0.2 && u->wep.Wep_status == Wep_on_swing )
		||
		(u->wep.Wep_status == Wep_on_dns && u->wep.stance == 1) ) 
		u->wep.Wep_breaks_guard = true;
	else
		u->wep.Wep_breaks_guard = false;

	if ( fabs( u->wep.angle_speed ) > 0.001 ) {
		u->wep.angle += u->wep.angle_speed;
		u->wep.x = u->wep.WD->length * cos(u->wep.angle) + u->x;
		u->wep.y = u->wep.WD->length * sin(u->wep.angle) + u->y;
	}
 }