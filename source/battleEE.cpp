#include "StdAfx.h"
#include "battleEE.h"
#include <math.h>

battleEE::battleEE(int *a_in, HitEffect * hef_in, bool *LBDown_in, bool *RBDown_in,
				   Arrow *arw_in, int * arw_counter_in ){
	next_effect_counter = a_in;
	hef = hef_in;
	bWP = battleWeaponActionPreset();
	bMP = battleMovePreset();

	LButtonDown = LBDown_in;
	RButtonDown = RBDown_in;

	bEE_arw = arw_in;
	arw_counter = arw_counter_in;
}

battleEE::~battleEE(void){}

/////////////////////////////////////
//Calculates the stamina regeneration of player unit A
/////////////////////////////////////
/*
				Not in use
*/
void battleEE::CalcStamina(Unit *A){
	if ( A->wep.stance == 1 ) {
		A->stamina += 0.1;
		A->stamina -= ((double)fabs(A->wep.old.x - A->wep.x))/20000.0;
		A->stamina -= ((double)fabs(A->wep.old.y - A->wep.y))/20000.0;
	}
	else if ( A->wep.stance == 2 ) {
		A->stamina += 0.1;
	}
	else if ( A->wep.stance == 0 ) {
		A->stamina += 0.5;
	}
	else 
		exit(0);		

	if ( A->stamina < 0 ) {
		A->stamina = 0;
		A->wep.next_stance = 0;
		A->wep.stance = 0;
		A->wep.stance_shift = true;
	}
	else if ( A->stamina > 100 ) 
		A->stamina = 100;

}

//////////////////////////////////////
//A->x to B.x, cut it into 4 and pick 3 intermediate points
// for collision detection
//////////////////////////////////////
void battleEE::CollisionPoints(Unit *u) {

	switch(u->wep.weapon_type){
		case 5:
			u->wep.c[5].x = u->wep.x;
			u->wep.c[5].y = u->wep.y;

			u->wep.c[0].x = u->wep.pulled_p.x + (u->wep.x - u->wep.pulled_p.x) / 6.0;
			u->wep.c[0].y = u->wep.pulled_p.y + (u->wep.y - u->wep.pulled_p.y) / 6.0;

			u->wep.c[1].x = u->wep.pulled_p.x + 2.0*(u->wep.x - u->wep.pulled_p.x) / 6.0;
			u->wep.c[1].y = u->wep.pulled_p.y + 2.0*(u->wep.y - u->wep.pulled_p.y) / 6.0;

			u->wep.c[2].x = u->wep.pulled_p.x + 3.0*(u->wep.x - u->wep.pulled_p.x) / 6.0;
			u->wep.c[2].y = u->wep.pulled_p.y + 3.0*(u->wep.y - u->wep.pulled_p.y) / 6.0;

			u->wep.c[3].x = u->wep.pulled_p.x + 4.0*(u->wep.x - u->wep.pulled_p.x) / 6.0;
			u->wep.c[3].y = u->wep.pulled_p.y + 4.0*(u->wep.y - u->wep.pulled_p.y) / 6.0;

			u->wep.c[4].x = u->wep.pulled_p.x + 5.0*(u->wep.x - u->wep.pulled_p.x) / 6.0;
			u->wep.c[4].y = u->wep.pulled_p.y + 5.0*(u->wep.y - u->wep.pulled_p.y) / 6.0;
			break;
		case 4:
		case 1:
			u->wep.c[3].x = u->wep.x;
			u->wep.c[3].y = u->wep.y;

			u->wep.c[2].x = u->wep.pulled_p.x + (u->wep.x - u->wep.pulled_p.x) / 4.0;
			u->wep.c[2].y = u->wep.pulled_p.y + (u->wep.y - u->wep.pulled_p.y) / 4.0;

			u->wep.c[1].x = u->wep.pulled_p.x + 2.0*(u->wep.x - u->wep.pulled_p.x) /4.0;
			u->wep.c[1].y = u->wep.pulled_p.y + 2.0*(u->wep.y - u->wep.pulled_p.y) / 4.0;

			u->wep.c[0].x = u->wep.pulled_p.x + 3.0*(u->wep.x - u->wep.pulled_p.x) / 4.0;
			u->wep.c[0].y = u->wep.pulled_p.y + 3.0*(u->wep.y - u->wep.pulled_p.y) / 4.0;
			break;

		case 2:
			u->wep.c[2].x = u->wep.pulled_p.x + 2.0*(u->wep.x - u->wep.pulled_p.x) / 13.0;
			u->wep.c[2].y = u->wep.pulled_p.y + 2.0*(u->wep.y - u->wep.pulled_p.y) / 13.0;

			u->wep.c[1].x = u->wep.pulled_p.x + 6.0*(u->wep.x - u->wep.pulled_p.x) / 13.0;
			u->wep.c[1].y = u->wep.pulled_p.y + 6.0*(u->wep.y - u->wep.pulled_p.y) / 13.0;

			u->wep.c[0].x = u->wep.pulled_p.x + 10.0*(u->wep.x - u->wep.pulled_p.x) / 13.0;
			u->wep.c[0].y = u->wep.pulled_p.y + 10.0*(u->wep.y - u->wep.pulled_p.y) / 13.0;

			break;
	}
}

///////////////////////////////////
//Detects CollisionS
//          - distance based -
//Weapon vs Unit
//Weapon vs Weapon
///////////////////////////////////
void battleEE::CollisionDetection(Unit *u, Unit *ou, Arrow *arw){
	double hit_radius;
	/////////////////////////////////////////////
	//unit position adjustment for better weapon position
	//  restored later
	if ( NON_PULLED_WEP_U ) {
		if ( u[0].team == 0) {
			for ( int i =0 ; i <= MAX_NUM_UNIT; i++ ) {
				u[i].x -= unit_position_adj_x;
				u[i].y -= unit_position_adj_y;
			}
		}
		else {
			for ( int i =0 ; i <= MAX_NUM_UNIT; i++ ) {
				u[i].x += unit_position_adj_x;
				u[i].y -= unit_position_adj_y;
			}
		}
	}

	if ( NON_PULLED_WEP_OU ) {
		if ( ou[0].team == 0) {
			for ( int i =0 ; i <= MAX_NUM_UNIT; i++ ) {
				ou[i].x -= unit_position_adj_x;
				ou[i].y -= unit_position_adj_y;
			}
		}
		else {
			for ( int i =0 ; i <= MAX_NUM_UNIT; i++ ) {
				ou[i].x += unit_position_adj_x;
				ou[i].y -= unit_position_adj_y;
			}
		}
	}
	
	////////////////////////////////
	//Weapon vs Unit
	//Collision detection on each unit's weapon vs other unit
	for ( int i=0; i <= MAX_NUM_UNIT; i++ ) {
		if ( u[i].dead || !u[i].active )
			continue;
		CollisionPoints(&u[i]);

		dummy = u[i].wep.WD->edges;
		hit_radius = 20.0;	

		//for Dsah n S mode
		if ( u[i].wep.Wep_status == Wep_on_dns &&
			u[i].wep.stance == 1 ) {
			dummy++;
			if ( u[i].wep.weapon_type != Unit_dagger ){
				hit_radius = 40.0;
			}
		}

		//if ( (u[i].wep.on_swing && abs(u[i].wep.angle_speed) > 0.1) 
		if ( is_atk(u[i]) ) {
			for ( int j = 0; j <= MAX_NUM_UNIT; j++ ) {				
				if (!ou[j].dead  && ou[j].active
					&&
					( !ou[j].wep.Wep_status == Wep_on_guard || u[i].wep.Wep_breaks_guard )
					&&
					( fabs(u[i].x - ou[j].x) < u[i].wep.WD->length )
					&&
					( fabs(u[i].y - ou[j].y) < u[i].wep.WD->length )
				) {

					for (int k = 0; k < dummy; k ++ ){				
						if ( dist( u[i].wep.c[k], ou[j] )
							< hit_radius  ) {
								//hit !
								ou[j].hit = true;
								
								hef[*next_effect_counter].timer = EFFECT_DURATION/2;
								hef[*next_effect_counter].pos.x = (u[i].wep.c[k].x + ou[j].x ) / 2.0;
								hef[*next_effect_counter].pos.y = (u[i].wep.c[k].y + ou[j].y ) / 2.0;
								
								if (ou[j].wep.Wep_status == Wep_on_guard ) { // hit over opponent's guard
									hef[*next_effect_counter].type = Hit_GuardBreak; 
									ou[j].stat_currentHP -= 2;//0.5;
								}
								else if (u[i].wep.Wep_status == Wep_on_dns) {
									if ( u[i].team == 0 )
										hef[*next_effect_counter].type = Hit_DnSB; 
									else
										hef[*next_effect_counter].type = Hit_DnSG; 
									ou[j].stat_currentHP -= (u[i].wep.weapon_type == 1 ? 20 : 10);
								}								
								else { 
									hef[*next_effect_counter].type = Hit_Normal;
									ou[j].stat_currentHP -= u[i].wep.weapon_type == 1 ?  8 : 4; //4;
								}
							
								*next_effect_counter += 1;
								*next_effect_counter %= EFFECT_NUM;

								//the unit being hit is moved by the opponent's weapon. 
								if ( ou[j].wep.weapon_type != Unit_gsword 
									&&
									(  u[i].wep.weapon_type == Unit_spear 
										|| 
									   (u[i].wep.weapon_type == Unit_gsword && u[i].wep.Wep_status == Wep_on_dns  ) 
									)) {
									ou[j].x = u[i].wep.c[k].x;
									ou[j].y = u[i].wep.c[k].y;
								}
								else {								
									if ( u[i].wep.c[k].x > ou[j].x )
										ou[j].x -= 3.0;
									else 
										ou[j].x += 3.0;

									if ( u[i].wep.c[k].y > ou[j].y )
										ou[j].y -= 3.0;
									else 
										ou[j].y += 3.0;
								}

								//Dasn N Slash Charge cancel
								if ( ou[j].wep.Wep_status == Wep_on_dns &&
									ou[j].wep.stance_charge < 100  ) {
									ou[j].ACT_type = ACT_idle;
									ou[j].wep.stance_charge = 0;
									ou[j].wep.Wep_status = Wep_on_idle;
									ou[j].AI_timer = AI_period -2;
								}
								//Bow - when hit, it just fires. 
								if ( ou[j].wep.weapon_type == 3 &&
									 ou[j].arwp != NULL ) {
										 ou[j].ACT_type = ACT_bowfire;
								}
						}		
					}
				}
			}
		}
	}

	////////////////////////////////
	//Weapon vs Weapon on guard stance
	for ( int i = 0; i <= MAX_NUM_UNIT; i++ ) {
		if ( u[i].dead || !u[i].active )
			continue;
		if ( u[i].wep.Wep_status == Wep_on_guard ){
			for ( int l =0; l <= MAX_NUM_UNIT; l++ ) {
				if (   !ou[l].active
					|| ou[l].dead 
					|| !is_atk(ou[l])
					|| dist(u[i],ou[l]) > 300.0  )
					continue;

				//ou[l] is the unit wielding the weapon
				for (int j = 0; j < 3; j ++ ){
					for ( int k = 0; k < 3; k ++ ) {
						if ( dist( u[i].wep.c[j], ou[l].wep.c[k]) < 20.0  ) {

							hef[*next_effect_counter].timer = EFFECT_DURATION/2;
							hef[*next_effect_counter].pos.x = (u[i].wep.c[j].x + ou[l].wep.c[k].x ) / 2.0;
							hef[*next_effect_counter].pos.y = (u[i].wep.c[j].y + ou[l].wep.c[k].y ) / 2.0;								
							hef[*next_effect_counter].type = Hit_Guarded; 							
							*next_effect_counter += 1;
							*next_effect_counter %= EFFECT_NUM;

							Wep_Wep_Collision_pushback = 2.5;//5.5;

							if ( u[i].x > ou[l].x )
								u[i].x += Wep_Wep_Collision_pushback;
							else 
								u[i].x -= Wep_Wep_Collision_pushback;

							if ( u[i].y > ou[l].y ) 
								u[i].y += Wep_Wep_Collision_pushback;	
							else 
								u[i].y -= Wep_Wep_Collision_pushback;

							j = 100;
							k = 100;
						}
							
						///fun idea, but does not seem to work well
						// simulates rebound of the weapon
						//if (abs(u[i].wep.angle_speed) > 0.10 ) {
						/*
							if ( u[i].wep.c[j].x > A->wep.x )
								A->wep.x -= 10.0;
							else 
								A->wep.x += 10.0;

							if ( u[i].wep.c[j].y > A->wep.y )
								A->wep.y -= 10.0;
							else 
								A->wep.y += 10.0;
						*/					
					}					
				}
			}
		}
	}

	speed_modifier = 0.0;
	
	///////////////////////
	//Arrow Collision
	for (int i = 0 ; i < MAXARROW; i++ ) {
		if ( arw[i].active && arw[i].flying ) {
			for ( int j = 0; j <= MAX_NUM_UNIT; j++ ) {
				if ( !u[j].active || u[j].dead || arw[i].team == u[j].team )
					continue;
				//vs  Weapon
				for (int k = 0; k < u[j].wep.WD->edges; k ++ ){
					if ( dist( arw[i].c, u[j].wep.c[k]) < 20.0 
						&&
						(u[j].wep.Wep_status == Wep_on_guard || u[j].wep.weapon_type ==4 ) ) {
						
						if ( arw[i].speed > 0.5 ) {
							u[j].hit = true;
							u[j].stat_currentHP -= 10*arw[i].speed;
						}

						arw[i].speed *= speed_modifier;
						//arw[i].active = false;
						u[j].x += (arw[i].front.x - arw[i].back.x)/10.0;
						u[j].y += (arw[i].front.y - arw[i].back.y)/10.0;

						hef[*next_effect_counter].timer = EFFECT_DURATION/2;
						hef[*next_effect_counter].pos.x = (u[j].wep.c[k].x ) ;
						hef[*next_effect_counter].pos.y = (u[j].wep.c[k].y ) ;								
						hef[*next_effect_counter].type = Hit_Guarded; 							
						*next_effect_counter += 1;
						*next_effect_counter %= EFFECT_NUM;

						j = MAX_NUM_UNIT+100;
						break;
					}
				}
				//vs  Unit
				if ( arw[i].team != u[j].team && dist( arw[i].c, u[j]) < 20.0  ) {
					u[j].hit = true;
					u[j].stat_currentHP -= 55;
					arw[i].speed *= speed_modifier; //.active = false;
					//arw[i].speed -= 0.1;// 1.0
					u[j].x += (arw[i].front.x - arw[i].back.x)/6;
					u[j].y += (arw[i].front.y - arw[i].back.y)/6;

					hef[*next_effect_counter].timer = EFFECT_DURATION;
					hef[*next_effect_counter].pos.x = (u[j].x ) ;
					hef[*next_effect_counter].pos.y = (u[j].y ) ;								
					hef[*next_effect_counter].type = Hit_Normal; 							
					*next_effect_counter += 1;
					*next_effect_counter %= EFFECT_NUM;

					j = MAX_NUM_UNIT+100;
				}
			}
		}
	}
	
	/////////////////////////////
	//restoring the modified value
	if ( NON_PULLED_WEP_U ) {
		if ( u[0].team == 0) {
			for ( int i =0 ; i <= MAX_NUM_UNIT; i++ ) {
				u[i].x += unit_position_adj_x;
				u[i].y += unit_position_adj_y;
			}
		}
		else {
			for ( int i =0 ; i <= MAX_NUM_UNIT; i++ ) {
				u[i].x -= unit_position_adj_x;
				u[i].y += unit_position_adj_y;
			}
		}
	}

	if ( NON_PULLED_WEP_OU ) {
		if ( ou[0].team == 0) {
			for ( int i =0 ; i <= MAX_NUM_UNIT; i++ ) {
				ou[i].x += unit_position_adj_x;
				ou[i].y += unit_position_adj_y;
			}
		}
		else {
			for ( int i =0 ; i <= MAX_NUM_UNIT; i++ ) {
				ou[i].x -= unit_position_adj_x;
				ou[i].y += unit_position_adj_y;
			}
		}
	}
}

//////////////////////////
// Unit Collision Checker
/////////////////////////
void battleEE::UnitCollisionChecker(Unit *u, Unit *ou){
	double d;

	//maintains the distance between the Units
	for ( int i = 0 ; i <= MAX_NUM_UNIT ; i ++ ) {
		if ( !u[i].active || u[i].dead )
			continue;
		//vs. Unit u
		for ( int j = 0 ; j <= MAX_NUM_UNIT ; j ++ ) {
			if ( !ou[j].active || ou[j].dead || &ou[j] == &u[i])
				continue;
			d = dist(u[i], ou[j]);
			if ( d < 40.0 ) {
				if ( u[i].x > ou[j].x ) {
					u[i].x += 2;
					ou[j].x -= 2;
				}
				else {
					u[i].x -= 2;
					ou[j].x += 2;
				}

				if ( u[i].y > ou[j].y ) {
					u[i].y += 2;
					ou[j].y -= 2;
				}
				else {
					u[i].y -= 2;
					ou[j].y += 2;
				}
			}
		}
	}
}

//////////////////////////////
//Handles the user mouse-input which controls the postion of weapon
//Handles the position of user weapon ( which is at the cursor position )
///////////////////////////////
void battleEE::Move_UW(Unit *A, POINT pt_cur){
	
	double distAB;
	double weapon_speed;
	double len_min, len_max;

	A->wep.old.x =A->wep.x;
	A->wep.old.y =A->wep.y;

	/////////////////////////
	// Manual control
	/////////////////////////
	// After changing the ' bool on_swing ' parameter into ' int Wep_status ' 
	//   the following part was not tested accordingly
	if ( false ){//A->ACT_type == 0 ){//A->wep.stance == 1 ) {
	
		A->wep.x = pt_cur.x + A->x ;
		A->wep.y = pt_cur.y + A->y ;

		weapon_speed = 10.0;

		if (A->wep.stance_shift)
			weapon_speed = 5.0;
		else if ( A->wep.on_hit )
			weapon_speed = 0.0;
		else if ( A->wep.stance == 1  )
			weapon_speed = 10.0;
		else if ( A->wep.stance == 2  )
			weapon_speed = 10.0;

		//bound the movement by the weapon speed
		distAB = dist((*A), A->wep.old) ; //sqrt(pow((A->wep.x - A->wep.old.x),2) + pow((A->wep.y - A->wep.old.y),2) );

		if ( distAB > weapon_speed ) {
			A->wep.x = A->wep.old.x + weapon_speed*(A->wep.x-A->wep.old.x) / distAB;
			A->wep.y = A->wep.old.y + weapon_speed*(A->wep.y-A->wep.old.y) / distAB;	
		}		

		//reposition the end of weapon so its within the length		
		switch(A->wep.weapon_type){
			case 1:
				len_max = A->wep.WD->length;
				len_min = A->wep.WD->length;//25.0;
				break;
			case 2:
				len_max = A->wep.WD->length;
				len_min = 80.0;
				break;
			case 3:
				len_max = A->wep.WD->length;
				len_min = A->wep.WD->length;
				break;
		}

		BoundUp(len_max, &A->wep.x, &A->wep.y, A->x, A->y);
		BoundDown(len_min, &A->wep.x, &A->wep.y, A->x, A->y);

		//determines whether the weapon is on swing based on its movement speed
		distAB = dist(A->wep, A->wep.old);//sqrt(pow((A->wep.x - A->wep.old.x),2) + pow((A->wep.y - A->wep.old.y),2) );

		if (distAB > 8) 
			A->wep.Wep_status = Wep_on_swing;
		
		//pulled weapon for spear mode
		if ( A->wep.weapon_type == 2 ) {
			distAB = dist(A->wep, (*A) );
			A->wep.pulled = A->wep.WD->length - distAB;
			A->wep.pulled_p.x = A->x - (A->wep.x - A->x)*(A->wep.pulled/(A->wep.WD->length-A->wep.pulled));
			A->wep.pulled_p.y = A->y - (A->wep.y - A->y)*(A->wep.pulled/(A->wep.WD->length-A->wep.pulled));
		}
		else {
			A->wep.pulled_p.x = A->x;
			A->wep.pulled_p.y = A->y;
		}		
	}

	////////////////////////////////
	// Preset Control
	////////////////////////////////
	else {

		/*
		Unit B;
		B.x = pt_cur.x + A->x-WWAD;
		B.y = pt_cur.y + A->y-WHAD;

		B.wep.c[1].x  = B.x; //guard point at this position
		B.wep.c[1].y  = B.y; //guard point at this position
	
		A->Target = &B;
		*/
	
		A->Target->x = pt_cur.x + A->x-WWAD;
		A->Target->y = pt_cur.y + A->y-WHAD;

		A->Target->wep.c[1].x  = A->Target->x;
		A->Target->wep.c[1].y  = A->Target->y;

		Move_MW(A);
	}

	if ( A->arwp != NULL )
		Move_LoadedArrow(A);
}

/////////////////////////////
// Moves Weapon of u ( AI Units )
/////////////////////////////
void battleEE::Move_MW(Unit * u){

	switch(u->wep.weapon_type) {
		
		case Unit_dagger:
			if ( u->Target != NULL ) {
				u->wep.x = u->Target->x;
				u->wep.y = u->Target->y;
			}
		case Unit_gsword:
		case Unit_spear:
		case Unit_sword: {
			switch(u->ACT_type) {
				case ACT_swing:
					bWP.Atk_Swing(u);
					break;
				case ACT_hswing:
					bWP.Atk_HSwing(u);
					break;
				case ACT_fswing:
					bWP.Atk_FSwing(u);
					break;
				case ACT_guard:
					bWP.Rdy_Guard(u);
					break;
				case ACT_poke:
					bWP.Atk_Poke(u);
					break;
				case ACT_dashNslash:
					bWP.Atk_DashNSlash(u);
					break;
				case ACT_spin:
					bWP.Atk_Spin(u);
					break;
				case ACT_idle:
					u->wep.Wep_status = Wep_on_idle;
					if ( u->userunit && ( (*LButtonDown) || (*RButtonDown )) )
						bWP.Rdy_Idle_UserUnit(u, (*LButtonDown), (*RButtonDown));
					else
						bWP.Rdy_Idle(u);
					break;
			}
			break;
		}
		case Unit_bow:
			switch(u->ACT_type){
			case ACT_bowaim:
				if ( u->arwp == NULL ) {
					for ( int j =0; j < MAXARROW; j++ ) { 
						(*arw_counter)++;
						(*arw_counter) %= MAXARROW;
						if (!bEE_arw[(*arw_counter)].active ) {
							u->wep.x = u->Target->x; //the target position
							u->wep.y = u->Target->y; 
							Load_Arrow(u,bEE_arw,(*arw_counter)++);			
							break;
						}
					}
				}
				break;
			case ACT_bowfire:
				Fire_Arrow(u);
				CALL_uAI; //defined in battleWeaponActionPreset
				break;
			case ACT_idle:
				if ( u->arwp != NULL )  {
					u->arwp->active = false;
					u->arwp = NULL;
				}
				break;
			}

			if ( u->arwp != NULL ) {
				u->wep.x = u->Target->x;
				u->wep.y = u->Target->y;

				Move_LoadedArrow(u);
			}
			break;
	}
	
	////////////////////////
	//stance reset 
	///////////////////////
	if ( u->wep.Wep_status != Wep_on_dns ) {
		u->wep.stance = 0 ;
		u->wep.stance_shift = false;
	}
	
	if ( true ) { //for the cases where no action is chosen
		BoundUp(u->wep.WD->length - u->wep.pulled, &u->wep.x, &u->wep.y, u->x, u->y);
		BoundDown(u->wep.WD->length - u->wep.pulled, &u->wep.x, &u->wep.y, u->x, u->y);
	}
}

/////////////////////////////
// Moves Arrow on the Bow
/////////////////////////////
void battleEE::Move_LoadedArrow(Unit *A) {
	double distAB = dist(A->wep, (*A) );

	A->arwp->back.x = A->x;
	A->arwp->back.y = A->y;

	A->arwp->front.x = A->wep.x;
	A->arwp->front.y = A->wep.y;

	BoundUp(60 * A->arwp->counter,   &A->arwp->front.x, &A->arwp->front.y, A->arwp->back.x, A->arwp->back.y);
	BoundDown(60 * A->arwp->counter, &A->arwp->front.x, &A->arwp->front.y, A->arwp->back.x, A->arwp->back.y);

	distAB = 60*(1.0 -  A->arwp->counter);
		
	A->arwp->back.x = A->arwp->back.x - (A->arwp->front.x - A->arwp->back.x)*(distAB/(60.0-distAB));
	A->arwp->back.y = A->arwp->back.y - (A->arwp->front.y - A->arwp->back.y)*(distAB/(60.0-distAB));
}


/////////////////////////////
// Load Arrow to Bow !
/////////////////////////////
void battleEE::Load_Arrow( Unit *u, Arrow* arw, int nc){

	u->arwp = &arw[nc];
	u->arwp->active = true;
	u->arwp->onbow  = true;
	u->arwp->flying = false;
	u->arwp->counter = 1.2;
	u->arwp->team = u->team;
}

/////////////////////////////
// pull back string and Arrow on the Bow !
/////////////////////////////
void battleEE::Chrg_Arrow(Arrow * arw){
	for ( int i =0; i < MAXARROW; i++ ) {
		if ( arw[i].active && arw[i].onbow ){
			if ( arw[i].counter > 0.7 ) 
				arw[i].counter -= 0.015;
		}
	}
}

/////////////////////////////
// Fire Arrow from Bow !
/////////////////////////////
void battleEE::Fire_Arrow( Unit *u ){
	if ( u->arwp != NULL ) 
	{
		if (u->arwp->counter <= 0.7 ) {
			u->arwp->speed = 1.3;
		}
		else {
			if ( u->arwp->counter > 0.9 ){
				u->arwp->speed = 0.0; //no strafing when being hti multiple times
			}
			else {
				u->arwp->speed = 2.7  - 2*u->arwp->counter;
			}
		}
		u->arwp->onbow  = false;
		u->arwp->flying = true;
		//u->arwp->speed = 2.0;
		//u->ACTION_aim = false; // arrow is fired with this
		u->arwp = NULL;
	}	
}

/////////////////////////////
// Move Arrows
/////////////////////////////
void battleEE::Move_Arrow(Arrow* arw){

	double dx, dy;

	for ( int i =0; i < MAXARROW; i++ ) {
		if ( arw[i].active ){
			if ( arw[i].flying  ) {
				dx = 0.4*(arw[i].front.x - arw[i].back.x)*arw[i].speed;
				dy = 0.4*(arw[i].front.y - arw[i].back.y)*arw[i].speed;

				arw[i].speed -= 0.027;

				if (arw[i].speed < 0.05 ) {
					arw[i].active = false;
				}
				else {
					arw[i].front.x += dx;
					arw[i].front.y += dy;
					arw[i].back.x += dx;
					arw[i].back.y += dy;
					//collision point.
					arw[i].c.x =  arw[i].front.x + 0.17*(arw[i].back.x - arw[i].front.x);
					arw[i].c.y =  arw[i].front.y + 0.17*(arw[i].back.y - arw[i].front.y);
				}
			}
			else { //arrow on ground or on bow

			}
		}
	}
}

/////////////////////////////
// Move_A, the user's unit
//////////////////////////
void battleEE::Move_A(Unit *A, POINT pt_cur,//mouse click-move can be implemented... 
	bool move_a, bool move_w, bool move_d , bool move_s){

	/*
	if ( A->roll && A->stamina > 10 ) {
		A->speed = 6;
		A->roll = false;
	}
	else
	{
		if ( A->speed > A->max_speed ) 
			A->speed -= 0.3;
		else {

			if ( A->ACT_type == ACT_fswing ||
				 A->ACT_type == ACT_hswing )
				 //||	 A->ACT_type == ACT_poke )
				A->speed = 01.0*A->max_speed;	
			else if ( A->ACT_type == ACT_guard ) 
				A->speed = 0.5*A->max_speed;	
			else if ( A->ACT_type == ACT_poke ) 
				A->speed = 0.0;
			else
				A->speed = A->max_speed;
		}
	}
	*/

	A->difM.x = 0;
	A->difM.y = 0;

	if ( move_a  ) {
		//A->x -= A->speed;
		A->direction = 1;
		A->difM.x += A->speed;
	}
	if ( move_d ) {
		//A->x += A->speed;
		A->direction = 3;
		A->difM.x -= A->speed;
	}
	if ( move_w  ) {
		//A->y -= A->speed;
		A->direction = 2;
		A->difM.y += A->speed;
	}
	if ( move_s  ) {
		//A->y += A->speed;
		A->direction = 0;
		A->difM.y -= A->speed;
	}
	Move_M( A );
}

//////////////////////////////
// Move_M : moves monsters towards difM direction.
//			bouned by its speed
/////////////////////////////
void battleEE::Move_M(Unit *u){
	if ( !u->active )
		return;

	switch(u->MV_type) {
		case MV_to_T:
			bMP.Mv_to_TargetU(u,		u->MV_dist);
			break;

		case MV_dist_T:
			bMP.Mv_dist_TargetU(u,	u->MV_dist);
			break;

		case MV_away_T_diag:
			bMP.Mv_diagaway_TargetU(u,	u->MV_clock_wise);
			break;
			
		case MV_to_T_diag:
			bMP.Mv_diag_TargetU(u,	u->MV_clock_wise);
			break;

		case MV_cir_T:
			bMP.Mv_circulate_TargetU(u, u->MV_dist,u->MV_clock_wise);
			break;

		case MV_to_T_g:
			bMP.Mv_to_TargetU_Guard(u);
			break;

		case MV_to_P_march:
		case MV_to_P:
			bMP.Mv_to_Position(u,	&u->MV_dest);
			break;

		case MV_cir_P:
			bMP.Mv_circulate_TargetU(u, u->MV_dist,u->MV_clock_wise);
			break;

		case MV_death_ani:
			bMP.Mv_DA(u);
			break;

		case MV_userunit:
				break;
	}

	u->old.x = u->x;
	u->old.y = u->y;

	if ( u->roll ) {
		u->speed = 50.0;
		u->roll = false;
	}
	else if (u->speed > u->max_speed ){
		u->speed -= 5.6;
	}
	else {
			
		if ( u->MV_type == MV_to_P_march && u->ACT_type == ACT_idle ){
		}			
		else if ( u->wep.weapon_type == 4 ) {
			if ( u->wep.Wep_status == Wep_on_dns )  {
				//u->speed = u->max_speed*0.8;
				u->speed = u->max_speed*(0.5+(100.0 - u->wep.stance_charge)/200.0);
			}
			else
				u->speed = u->max_speed;
		}
		else { 
			if ( u->wep.Wep_status == Wep_on_swing ) 
				u->speed += (0.60 *u->max_speed - u->speed) / 10.0;
			else if ( u->ACT_type == ACT_guard )
				u->speed += (0.5*u->max_speed - u->speed)   / 10.0;
			else if ( u->ACT_type == ACT_poke || u->ACT_type == ACT_dashNslash )
				u->speed = 0.0;//+= (0.0*u->max_speed - u->speed)   / 10.0;
			else if ( u->ACT_type == ACT_bowaim )
				u->speed += (0.2 *u->max_speed - u->speed)  / 10.0;
			else 
				u->speed += (     u->max_speed - u->speed)  / 10.0;
		}
	}

	// Slow effect from the hits.
	if ( u->status_slow > 0 ) {
		u->speed *= 0.6;
		u->status_slow -=  u->stat_recover_speed;
	}


	//Now moves with circular speed limit 
	dummy = sqrt( pow(u->difM.x, 2.0) + pow(u->difM.y, 2.0) ) ;
	if ( fabs(dummy) > 0.0001 ) {
			if ( u->difM.x > 0 ) {				
				u->x -= dummy > u->speed ? u->difM.x/dummy*u->speed : u->difM.x;
				u->direction = 1;
			}
			else if ( u->difM.x < 0 ){
				u->x -= dummy > u->speed ? u->difM.x/dummy*u->speed : u->difM.x;
				u->direction = 3;
			}

			if ( u->difM.y > 0 ){
				u->y -= dummy > u->speed ? u->difM.y/dummy*u->speed : u->difM.y;
				if ( fabs(u->difM.x) < 2*fabs(u->difM.y) )
					u->direction = 2;
			}
			else if ( u->difM.y < 0 ){
				u->y -= dummy > u->speed ? u->difM.y/dummy*u->speed : u->difM.y;
				if ( fabs(u->difM.x) < 2*fabs(u->difM.y) )
					u->direction = 0;
			}
	}

	////////////////////
	//Direction Setting of Units
	///////////////////
	
	if ( u->dead )
		return;

	//unit faces the weapon (can be cleaned)
	if ( u->ACT_type == ACT_fswing || (u->userunit && u->wep.Wep_status == Wep_on_swing)
			
		){
		if ( fabs( u->x - u->wep.x ) > fabs( u->y - u->wep.y ) ) {
			if ( u->x - u->wep.x  > 0 ) 
				u->direction = 1;
			else 
				u->direction = 3;
		}
		else {
			if ( u->y - u->wep.y  > 0 ) 
				u->direction = 2;
			else 
				u->direction = 0;
		}
	}
		
	//unit faces the Target (can be cleaned)
	else if ( !u->userunit 
			&& 
			( u->wep.Wep_status == Wep_on_swing || u->wep.Wep_status == Wep_on_poke || u->wep.Wep_status == Wep_on_guard 
			|| dist( (*u), (*u->Target)) < 100.0 
			|| ( u->wep.weapon_type == 3 && u->ACT_type == ACT_bowaim )
			|| (u->ACT_type == ACT_dashNslash && u->wep.stance == 1)) 
		){
		if ( fabs( u->x - u->Target->x ) > fabs( u->y - u->Target->y ) ) {
			if ( u->x - u->Target->x  > 0 ) 
				u->direction = 1;
			else 
				u->direction = 3;
		}
		else {
			if ( u->y - u->Target->y  > 0 ) 
				u->direction = 2;
			else 
				u->direction = 0;
		}
	}
}

///////////////////
// Macro, bounds the length of position by the input length, upper bound
////////////////////
inline void battleEE::BoundUp(double len_max, double* Ex, double *Ey, double Rx, double Ry){

	double distAB = sqrt(pow((*Ex - Rx),2) + pow((*Ey - Ry),2));
	if ( distAB > len_max ) {
		*Ex = Rx + len_max*(*Ex - Rx) / distAB;
		*Ey = Ry + len_max*(*Ey - Ry) / distAB;
	}
}

///////////////////
// Macro, bounds the length of position by the input length, lower bound
////////////////////
inline void battleEE::BoundDown(double len_min, double* Ex, double *Ey, double Rx, double Ry){

	double distAB = sqrt(pow((*Ex - Rx),2) + pow((*Ey - Ry),2));
	if ( distAB < len_min ) {
		*Ex = Rx + len_min*(*Ex - Rx) / distAB;
		*Ey = Ry + len_min*(*Ey - Ry) / distAB;
	}
}