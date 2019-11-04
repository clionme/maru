#include "StdAfx.h"
#include "microAI.h"

microAI::microAI(void){}
microAI::~microAI(void){}

////////////////
//  Marches to the target location
////////////////
void microAI::march(Unit *u){
	u->ACT_type = ACT_idle;
	u->MV_type = MV_to_P_march;
}

//////////////////////////
//Sword - Guard
//    stays within the range of secondary target 
//     and attacks the primary target. 
/////////////////////////
void microAI::guard_Sword(Unit *u, Unit *t) {
	if ( Common_PreScript(u) ) 
		return;

	if (  TdistG > 230 ) {
		u->AI_preset[0] = 0;
	}
	else if ( Gdist > TdistG ){
		u->AI_preset[0] = 1;
	}
	else if ( Tdist < u->wep.WD->length ) {
		u->AI_preset[0] = 2;
	}
	else {
		u->AI_preset[0] = 0;
	}
	
	if ( TdistG < 150 && Tdist < 150 && u->T_second->TargetedBy > 2  ){
		u->AI_preset[0] = 3;
	}

	switch( u->AI_preset[0] ){
		case 0:
			////////////////////////
			// Preset 0
			//		Situation : enemy is far away.
			//		Behavior : stays in front of archer with guard stance
			////////////////////////
			if ( Gdist < 150 ) {
				u->MV_type = MV_to_T_g;
				u->MV_dist = 100.0;
				Uguard;
			}
			else if (Gdist < 200 ){
				u->MV_dest.x = u->T_second->x;
				u->MV_dest.y = u->T_second->y;
				u->MV_type = MV_to_P;
				u->MV_dist = 100.0;
			}
			else {
				u->MV_dest.x = u->T_second->x;
				u->MV_dest.y = u->T_second->y;
				u->MV_type = MV_to_P;
				u->MV_dist = 0.0;
				Uidle;
			}
			break;

		case 1:
			////////////////////////
			// Preset 1
			//		Situation : enemy is nearer to guard target than guardian (this unit).
			//		Behavior : runs in between guarded unit and the enemy.
			////////////////////////
			if ( u->ACT_type == ACT_guard && Gdist > 100.0 ){
				Uidle;
			}
			else {
				u->MV_type = MV_to_T_g;
				u->MV_dist = 100.0;
			}
			break;

		case 2:
			////////////////////////
			// Preset 2
			//		Situation : enemy is within range of guardian and the unit being guarded is behind guardian.
			//		Behavior : hswing.
			////////////////////////
			u->MV_type = MV_to_T;
			u->MV_dist = 80.0;
			Uhswing;

		case 3:
			//////////////////////
			// Preset 3
			//		Situation : the place is overrun by the enemy 
			//			and the unit being guarded is in hopeless situation.
			//		Behavior : fswing.
			//////////////////////
			Uhswing;
			u->MV_type = MV_to_T_diag;
	}
	Common_PostScript(u,t);
}

/////////////////////////
//Sword - Simple Script
/////////////////////////
void microAI::simple_Sword(Unit *u, Unit *t){

	if ( Common_PreScript(u) ) 
		return;

	//swaps preset depending on the situation
	if ( u->AI_sequence == 0 ) {
		switch(u->TargetedBy) {
		case 0:
			if ( rand()%2 == 0 ) 
				u->AI_preset[0] = 6;
			else if ( rand()%2 == 0 ) 
				u->AI_preset[0] = 7;
			else 
				u->AI_preset[0] = 5;
			break;
		case 1:
			if ( rand()%2 == 0 ) 
				u->AI_preset[0] = 4;
			else 
				u->AI_preset[0] = 3;
			break;
		case 2:
			if ( rand()%2 == 0 )
				u->AI_preset[0] = 2;
			else 
				u->AI_preset[0] = 1;
			break;
		case 3:
		case 4:
		case 5:
			u->AI_preset[0] = 0;
			/*
			if ( rand()%3 == 0 ) 
				u->AI_preset[0] = 0;
			else if ( rand()%2 == 0 )
				u->AI_preset[0] = 2;
			else 
				u->AI_preset[0] = 1;
			*/
			break;
		}

		if ( u->Target->userunit ) 
			u->AI_preset[0] = rand()%5+3;
	}

	//aggressive when enemy is dagger unit
	if (   u->Target->wep.weapon_type == Unit_dagger 
		|| u->Target->wep.weapon_type == Unit_bow) {
		switch( rand()%3 ) {
		case 1:
			u->AI_preset[0] = 6;
			break;
		case 2:
			u->AI_preset[0] = 5;
			break;
		case 3:

			break;
		}
	}

	if ( u->wep.weapon_type == Unit_spear && u->AI_preset[0] == 7 ) 
		u->AI_preset[0]--;
	
	switch ( u->AI_preset[0]  ) { 
		////////////////////////
		// Preset 0-0
		//		Guard and run
		////////////////////////
		case 0:	
			u->MV_type = MV_to_P;//away_T_diag;
			u->MV_dest.x = u->Lcenter->x;
			u->MV_dest.y = u->Lcenter->y;

			switch( u->AI_sequence ) {
				
			case 0:
				Uidle
				u->AI_sequence++;
				break;
			case 1:
				if ( Tclose ) {
					Uguard
					//BusyCALL
					//u->MV_type = MV_away_T_diag;
				}
				else {
					
					Upoke			
					//u->MV_type = MV_away_T_diag;
					u->AI_sequence++;
				}
				break;
			}
			u->AI_sequence%=2;
			break;

		////////////////
		// Preset 0-1
		//		Guard and poke
		///////////////
		case 1:
			switch( u->AI_sequence ) {
				
			case 0:
				u->AI_sequence++;
				break;
			case 1:
				u->MV_type = MV_to_T_diag;
				u->MV_dist = u->Target->wep.WD->length ;//  140.0;
				u->ACT_type = ACT_idle;
			
				if ( t->ACT_type == ACT_guard ) {
					u->MV_type = MV_to_T;
					u->MV_dist = 0.0; 
					Ufswing
				}
				else if ( Tfswing ) {
					Upoke
					u->MV_type = MV_dist_T;
					u->MV_dist = u->Target->wep.WD->length; 
				}
				else 
					u->AI_sequence++;
				break;
			case 2:
				if (   dist( (*u), (*t) ) <  60.0 ){//&& u->wep.angle_speed < 0.2 ) {//&& !u[i].wep.on_swing ) 
					Upoke
					u->AI_sequence++;
				}
				else 
					u->MV_type = MV_to_T_diag;
				break;
			case 3:
				if (   dist( (*u), (*t) ) <  170.0 ) {
					Uguard
					AIDelay(2)
					u->AI_sequence++;
				}
				else {
					Uidle
					u->MV_type = MV_to_T_diag;
				}
				break;			
			}	
			u->AI_sequence%=4;
			break;

		////////////////////////
		// Preset 0-2
		//		Run + FSwing based attack
		//			if FSwing is hit, performs HSwing
		////////////////////////
		case 2:
			switch(u->AI_sequence) {
			case 0:
				if ( dist( (*u), (*t) ) < 160.0 ) {
					if ( t->ACT_type == ACT_fswing || 
						t->ACT_type == ACT_hswing ){//||
						 //t->ACT_type == ACT_poke){//wep.angle_speed > 0.1 ){//true ) {//t->ACT_type == ACT_guard ) {
						Uguard
						BusyCALL
					}
					else {
						Uidle
					}
					u->MV_type = MV_dist_T;
					u->MV_dist = 190.0;
				}
				else if (dist( (*u), (*t) ) >  400.0) {
					u->MV_type = MV_to_T_diag;//MV_to_T;//MV_cir_T;//;
					Uidle
				}
				else {
					Ufswing
					u->AI_sequence++;
				}
				break;
			case 1:
				if ( (u->wep.Wep_breaks_guard) ) {
					u->MV_type = MV_to_T;//MV_to_T;//MV_cir_T;//;
					u->MV_dist = 0.0;
					u->AI_sequence++;
				}
				else {
					u->MV_type = MV_dist_T;
					u->MV_dist = 160.0;
				}
				break;
			case 2:
				if ( (u->wep.Wep_breaks_guard) ) {
				}
				else {
					if ( dist((*t), (*u)) < 100 ) {
						Uhswing
						u->AI_sequence++;
					}
					else {
						u->MV_type = MV_dist_T;
						u->MV_dist = 200.0;
						u->AI_sequence+=3;
					}
				}
				break;
			case 3:
				if ( (u->wep.Wep_status == Wep_on_swing) ) {
					//u->MV_type = MV_to_T;//MV_to_T;//MV_cir_T;//;
					//u->MV_dist = 120.0;
					BusyCALL
				}
				else {
					if ( t->wep.Wep_status != Wep_on_poke ) 
						u->MV_clock_wise = circulate_direction(t,u);
					u->MV_type = MV_away_T_diag;
					u->AI_sequence++;
					u->AI_timer -= 2*AI_ACTION_period;
				}
				break;
			case 4:
				if ( dist((*t),(*u)) > 200 || rand()%9 == 8 ) 
					u->AI_sequence++;
				else
					u->AI_sequence--;
				break;
		}
		u->AI_sequence%=5;
		break;

		////////////////////////
		// Preset 0-3
		//		Hit & Run 
		//			does Fswing then runs away	
		////////////////////////
		case 3:
			switch(u->AI_sequence) {
			case 0:
				if ( Tclose ) {
					u->MV_type = MV_dist_T;
					u->MV_dist = 250.0;
					if ( TAattack ){//||
						 //t->ACT_type == ACT_poke){//wep.angle_speed > 0.1 ){//true ) {//t->ACT_type == ACT_guard ) {
						Uguard
						BusyCALL
					}
					else {
						Uidle
					}
				}
				else if (dist( (*u), (*t) ) >  200.0) {
					u->MV_type = MV_to_T_diag;//MV_to_T;//MV_cir_T;//;
					Uidle
				}
				else {
					Ufswing
					u->MV_type = MV_to_T_diag;
					u->AI_sequence++;
				}
				break;
			case 1:
				u->MV_type = MV_away_T_diag;
				Uidle;
				u->AI_sequence++;

				break;
		}
		u->AI_sequence%=2;
		break;

		////////////////////////
		// Preset 0-4
		//		Guard until enemy attack
		////////////////////////
		case 4:
			u->MV_type = MV_cir_T;
			u->MV_dist = 140.0;
			switch(u->AI_sequence ) {
			case 0:
				u->AI_sequence = 1;
				Uidle
				BusyCALL

				u->AI_var_wait = -4;
				break;
			case 1: //wait for enemy attack then use hswing then guard
				if ( t->Target != u && !t->userunit ){
					u->AI_sequence = 3;
					break;
				}
				if ( TAattack ) {
					u->AI_sequence = 3;
				}
				else if ( t->ACT_type != ACT_guard && Tdist < 200  ) {
					Uguard
					if ( u->AI_var_wait++ > 0 )
						u->AI_sequence++;
					//BusyCALL
				}
				else {
					Uhswing
					u->MV_type = MV_to_T_diag;
					//BusyCALL
				}
				break;
			
			case 2:
				if ( t->ACT_type == ACT_guard ||  t->ACT_type == ACT_idle  ) {
					u->AI_sequence = 0;
					BusyCALL
				}
				else {
					u->AI_sequence++;
					BusyCALL
				}
				break;

			case 3: //attack after the enemy attack ends
				if ( Tdist < 150  ) {
					if ( TAattack) {
						//wait until the attack ends
						Uguard
						BusyCALL
					}
					else {
						Upoke
						u->MV_type = MV_to_T;
						u->MV_dist = 0.0;
						u->AI_sequence = 4;
					}
				}
				else {
					Uidle
					u->MV_type = MV_to_T;
					u->MV_dist = 0.0;
					u->AI_sequence = 0;
				}
				break;
			
			case 4: //continue the attack with hswing
				if ( Tdist < 150  ) {
					Uhswing
					u->MV_type = MV_to_T;
					u->MV_dist = 0.0;
					u->AI_sequence = 0;
				}
				else {
					Uidle
					u->MV_type = MV_to_T;
					u->MV_dist = 0.0;
					u->AI_sequence = 0;
				}
				break;
			}
			u->AI_sequence%=5;
			break;

		////////////////////////
		// Preset 0-5
		//		Aggressive attack sequence
		//      guard->poke->fswing->hswing->guard
		////////////////////////
		case 5:
			switch(u->AI_sequence) {
			case 0: 
				//get close first
				if ( Tfar ) {
					u->MV_type = MV_to_T;
					u->MV_dist = 0.0;
					Uidle
				}
				else {
					u->MV_type = MV_to_T;
					u->MV_dist = 0.0;
					Uguard
					AIDelay(1)
					u->AI_sequence++;
				}
				break;
			case 1:
				//poke when close by
				if ( Tclose ) {
					Upoke					
					u->AI_sequence++;
				}
				else {
					u->AI_sequence--;
				}
				break;
			case 2:
				Ufswing
				u->AI_sequence++;
				u->MV_type = MV_to_T_diag;
				u->MV_dist = 0.0;
				break;
			case 3:
				Uhswing
				u->AI_sequence++;
				break;
			}
			u->AI_sequence%=4;
		break;
		
		////////////////////////
		// Preset 0-6
		//		Only attack sequence
		//      poke->fswing->hswing
		////////////////////////
		case 6:
			switch(u->AI_sequence) {
			case 0: 
				u->MV_type = MV_to_T;
				u->MV_dist = 0.0;
				//get close first
				if ( dist( (*u), (*t) ) > 250.0  ) {
					Uidle
				}
				else {
					Ufswing
					u->AI_sequence++;
				}
				break;
			case 1:
				//swing when close by
				if (  dist( (*u), (*t) ) > 120 ) {
					u->MV_type = MV_to_T_diag;
					Uidle
					u->AI_sequence--;
				}
				else {
					u->MV_type = MV_cir_T;
					u->MV_dist = 100.0;
					u->AI_sequence++;
					Uhswing
				}
				break;
			}
			u->AI_sequence%=2;
		break;

		////////////////////////
		// Preset 0-7
		//		DashNSlash
		//		 Runs towards the enemy and uses D-N-S
		////////////////////////
		case 7:	

			if ( (Tdist < 150 )
				&& (u->Target->Target == u || u->Target->userunit)
				&& (u->wep.stance_charge < 50) ) {
				u->AI_preset[0] = 6;
			}

			switch( u->AI_sequence ) {
			case 0:

				u->AI_sequence++;
				BusyCALL
				break;
			case 1:
				if ( (u->Target->Target != u && (Tdist < 200 ) )
					||
					 (u->Target->Target == u && (Tdist < 300 ) )
					) {
					Udns
					u->MV_type = MV_cir_T;
					u->MV_dist = 200;
					u->AI_sequence++;
					//no busycall here
				}
				else if (u->Target->Target == u && Tdist < 100) {
					//u->AI_sequence--;
					u->ACT_type = ACT_hswing;
					//BusyCALL
				}
				else {
					Uidle				
					u->MV_type = MV_to_T_diag;
					BusyCALL
				}
				break;
			case 2: //on DnS
				//When DnS casting is broken
				if ( u->wep.Wep_status == Wep_on_idle ) {
					if ( (dist( (*u), (*t) ) < 100 ) ){
						//target is close by
						Uhswing
							AIDelay(2)
						u->AI_sequence = 1;
						
					}
					else {
						Udns
					}
					break;
				}
				if ( u->wep.stance_charge < 50 ) {
					//maintain distance
					Udns
					u->MV_type = MV_cir_T;
					u->MV_dist = 200;
				}
				else {
					u->MV_type = MV_to_T_diag;
					u->AI_sequence++;
					AIDelay(2);
				}
				break;
			case 3:
				if ( u->ACT_type != ACT_guard && (dist( (*u), (*t) ) < 200 ) ) {
					u->MV_type = MV_away_T_diag;
					u->ACT_type = ACT_guard;
					AIDelay(1)
					u->AI_sequence++;
				}
				else {
					u->ACT_type = ACT_idle;
					BusyCALL
					u->AI_sequence++;
				}
				break;
			}
			u->AI_sequence%=4;
			break;

		///////
		// Spear
		///////
		case 9:
			//if target is close by
			if ( Tpokable ) {
				Uguard
			}
			//if target is far away
			else {
				Uspin
			}
		
		break;
	}
	
	Common_PostScript(u,t);
}

//////////////////////////
//Simple Script
//   Dagger
/////////////////////////
void microAI::simple_Dagger(Unit *u, Unit *t){

	if ( Common_PreScript(u) ) 
		return;

	if ( u->AI_sequence == 0) {
		if ( t->wep.weapon_type == 3 && u->TargetedBy == 0  ) //bow
			u->AI_preset[0] = 2;
		else {
			u->AI_preset[0] = rand()%2; // 
		}
	}

	//swaps preset depending on the situation
	//if ( u->AI_sequence == 0 ) {
	//	u->AI_preset[0] = 0;
		/*
		switch(u->TargetedBy) {
		case 0:
			if ( rand()%2 == 0 ) 
				u->AI_preset[0] = 6;
			else 
				u->AI_preset[0] = 5;
			break;
		case 1:
			if ( rand()%2 == 0 ) 
				u->AI_preset[0] = 4;
			else 
				u->AI_preset[0] = 3;
			break;
		case 2:
			if ( rand()%2 == 0 )
				u->AI_preset[0] = 2;
			else 
				u->AI_preset[0] = 1;
			break;
		case 3:
		case 4:
		case 5:
			u->AI_preset[0] = 0;
			/*
			if ( rand()%3 == 0 ) 
				u->AI_preset[0] = 0;
			else if ( rand()%2 == 0 )
				u->AI_preset[0] = 2;
			else 
				u->AI_preset[0] = 1;
			*/
		//	break;
		//}
	//}

	switch ( u->AI_preset[0]  ) { 
		////////////////////////
		// Preset 0-0
		//		DashNSlash
		//		 Runs towards the enemy and uses D-N-S
		////////////////////////
		case 0:	
			switch( u->AI_sequence ) {
			case 0:
				u->AI_sequence++;
				BusyCALL
				break;
			case 1:
				if ( (dist( (*u), (*t) ) < 200 ) ) {
					Udns
					u->MV_type = MV_cir_T;
					u->MV_dist = 200;
					u->AI_sequence++;
					//no busycall here
				}
				else {
					Uidle				
					u->MV_type = MV_to_T_diag;
					BusyCALL
				}
				break;
			case 2: //on DnS
				//When DnS casting is broken
				if ( u->wep.Wep_status == Wep_on_idle ) {
					if ( (dist( (*u), (*t) ) < 100 ) ){
						//target is close by
						Uhswing
							AIDelay(1)
						u->AI_sequence = 1;
						
					}
					else {
						Udns
					}
					break;
				}
				if ( u->wep.stance_charge < 50 ) {
					//maintain distance
					Udns
					u->MV_type = MV_cir_T;
					u->MV_dist = 200;
				}
				else {
					u->MV_type = MV_to_T_diag;
					u->AI_sequence++;
					AIDelay(2);
				}
				break;
			case 3:
				if ( (dist( (*u), (*t) ) < 200 ) ) {
					u->MV_type = MV_away_T_diag;
					u->ACT_type = ACT_guard;
					u->AI_sequence++;
					//BusyCALL
				}
				else {
					u->ACT_type = ACT_idle;
					//BusyCALL
					u->AI_sequence++;
				}
				break;
			}
			u->AI_sequence%=4;
			break;

		////////////////////////
		// Preset 0-1
		//		Anti - DnS ( or cast ) mode
		//		runs in and attacks to stop casting
		////////////////////////
		case 1:	
			u->MV_dist = 120;
			//Anti - DnS
			if ( t->wep.Wep_status == Wep_on_dns ) {
				u->MV_type = MV_to_T_diag;
				if ( dist( (*u), (*t) ) < 100 ) {
					Upoke
				}
				else { 
					//if far away, counter dns with dns
					
					if ( t->wep.stance_charge > 60 ) {
						Udns
					}
					else {
						u->MV_type = MV_cir_T;
						u->MV_dist = 220;
					}
				}
			}
			//DnS style attack sequence if target is not using dns
			else {

				switch( u->AI_sequence ) {
				case 0:
					u->AI_sequence++;
					BusyCALL
					break;
				case 1:
					if ( (dist( (*u), (*t) ) < 200 ) ) {
						Udns
						u->MV_type = MV_to_T;
						//u->MV_dist = 200;
						u->AI_sequence++;
					}
					else {
						Uguard				
						u->MV_type = MV_to_T_diag;
						//BusyCALL
					}
					break;
				case 2: //on DnS
					if ( u->wep.stance_charge < 50 ) {
						//maintain distance
						Udns
						u->MV_type = MV_cir_T;
						//u->MV_dist = 200;
					}
					else {
						u->MV_type = MV_to_T_diag;
						u->AI_sequence++;
						AIDelay(2);
					}
					break;
				case 3:
					if ( (dist( (*u), (*t) ) < 200 ) ) {
						u->MV_type = MV_away_T_diag;
						u->ACT_type = ACT_guard;
						u->AI_sequence++;
						//BusyCALL
					}
					else {
						u->ACT_type = ACT_idle;
						//BusyCALL
						u->AI_sequence++;
					}
					break;
				}
				u->AI_sequence%=4;
				break;
			}
			break;

		////////////////////////
		// Preset Dagger 0-2
		//		Anti - Archer mode
		//		tries to avoid other targets and focuses on the archer
		////////////////////////
		case 2:	
			//Target is archer.
			u->MV_type = MV_to_T_diag;
			switch( u->AI_sequence){
			case 0:
				u->AI_sequence++;
				break;
			case 1: 
				if ( Tdist < 200 ) { //close by, attack
					Upoke
				}
				else {
					Uguard
						u->AI_sequence++;
				}
			}
			u->AI_sequence %= 2;
			break;
	}

	Common_PostScript(u,t);
}


//////////////////////////
//Simple Script
//     BOW
/////////////////////////
void microAI::simple_Bow(Unit *u, Unit *t){

	if ( Common_PreScript(u) ) 
		return;

	u->MV_type = MV_dist_T;
	u->MV_dist = 700;
	u->ACT_type = ACT_idle;

	//switch AI preset 
	if (   (u->arwp != NULL && u->arwp->counter > 0.85) 
		|| (u->arwp == NULL) ) {
		switch(u->TargetedBy){
			case 0:
				u->AI_preset[0] = 0; //free to shoot. 
				break;
			case 1:
				//run from the target.
				//check which unit is chasing you then run or attack.
				if (   u->Target->Target == u 
					&& u->Target->TargetedBy + u->Target->TargetedBy_R > 1 ) 
					u->AI_preset[0] = 0; 
				else
					u->AI_preset[0] = 1; 
				break;
			default :
				//2+ units closing by, run toward ally unit near by or until enough distance is obtained
				if ( u->T_second == u ) //does not have guarding unit
					u->AI_preset[0] = 2; 
				else  //has the guard
					u->AI_preset[0] = 3; 
				break;
		}

		////////////////////
		// SGroup Level Control
		////////////////////
		if ( u->AI_preset[1] == GACT_HoldPosition )
			u->AI_preset[0] = 0;
	}

	switch(u->AI_preset[0]) {
		////////////////////////
		// Preset Bow 0-0
		//		Free Shot
		////////////////////////
		case 0:
		/*	if ( (u->ACT_type != ACT_bowaim && dist((*u), (*u->Target) ) < 400.0 )
				&& 
				u->x > WWAD+100 && u->x < WWAD+map_W - 100
				&&
				u->y > WHAD+100 && u->y < WHAD+map_H - 100
				) {
				u->MV_type = MV_dist_T;
				u->MV_dist = 500;
				u->ACT_type = ACT_idle;
				BusyCALL
			}
			else 
		*/		if ( dist((*u), (*u->Target) ) < 600.0 )//&& !u[i].wep.on_swing ) 
			{	
				if ( u->TargetedBy_R != 0 ) {
					u->MV_type = MV_cir_T;
				}
				else
					u->MV_type = MV_dist_T;
				u->MV_dist = 500;

				u->ACT_type = ACT_bowaim;
				if ( u->arwp != NULL && u->arwp->counter < 0.7 ) {
					u->ACT_type = ACT_bowfire;
				}
			}	
			else {
				u->MV_type = MV_dist_T;
				u->MV_dist = 500;
				u->ACT_type = ACT_idle;
				BusyCALL
			}
			break;

		////////////////////////
		// Preset Bow 0-1
		//		Check who is targetting the unit
		////////////////////////
		case 1:
			// if target's target is me
			//  or someone is targetting the target.
			// keep firing 
			if ( (u->Target->Target == u || u->Target->TargetedBy > 1 )&& dist((*u), (*u->Target) ) < 700.0 )
			{	
				u->MV_dist = 600;
				u->MV_type = MV_to_T_diag;
				u->ACT_type = ACT_bowaim;
				if ( u->arwp != NULL && u->arwp->counter < 0.7 ) {
					u->ACT_type = ACT_bowfire;
				}
			}
			else if ( u->dist_to_closest_enemy > 500 ) {
					if ( dist((*u), (*u->Target) ) < 700.0 ) {
						u->MV_dist = 700;
						u->MV_type = MV_cir_T;
						u->ACT_type = ACT_bowaim;
						if ( u->arwp != NULL && u->arwp->counter < 0.7 ) {
							u->ACT_type = ACT_bowfire;
						}
					}
					else {
						Uidle
						u->MV_type = MV_to_T;
					}
			}
			else { // run !
				u->MV_type = MV_away_T_diag;
				//fires what it has, then runs
				if ( u->arwp != NULL ) 
					u->ACT_type = ACT_bowfire;
				else 
					u->ACT_type = ACT_idle;
			}
		
			break;

		////////////////////////
		// Preset Bow 0-2
		//		Run away until enough distance
		////////////////////////
		case 2:
			if ( u->dist_to_closest_enemy > 700.0 ) {
				if ( dist((*u), (*u->Target) ) < 700.0 ) {
					u->ACT_type = ACT_bowaim;
					if ( u->arwp != NULL && u->arwp->counter < 0.7 ) {
						u->ACT_type = ACT_bowfire;
					}
				}
				else {
					u->ACT_type = ACT_idle;
					u->MV_type = MV_to_T_diag;
				}
			}
			else {
				u->ACT_type = ACT_idle;
				u->MV_type = MV_away_T_diag;
			}
			break;

		////////////////////////
		// Preset Bow 0-3
		//		Run around your guard until you have enough distance
		////////////////////////
		case 3:
			if ( u->dist_to_closest_enemy > 500.0 ) {
				if ( dist((*u), (*u->Target) ) < 700.0 ) {
					u->ACT_type = ACT_bowaim;
					if ( u->arwp != NULL && u->arwp->counter < 0.85 ) { //faster shot
						u->ACT_type = ACT_bowfire;
					}
				}
				else {
					u->ACT_type = ACT_idle;
					u->MV_type = MV_to_T_diag;
				}
			}
			else {
				u->ACT_type = ACT_idle;
				u->MV_dest.x = u->T_second->x;
				u->MV_dest.y = u->T_second->y;
				u->MV_type = MV_cir_P;
				u->MV_dist = 140;
			}
			break;
	}

	if ( u->AI_preset[1] == GACT_Stay_Close ){
		if ( dist( u->MV_dest, (*u) ) > 200 ) {
			
			u->MV_type = MV_to_P;
			u->MV_dist = 200.0;
		}
	}
}

/////////////////
// Determines in which direction the unit will circulate the target.
/////////////////
bool microAI::circulate_direction(Unit *t, Unit *u ){
	//the direction is selected based on the position of target's weapon

	//origin : target

	/*		unit	(x2,y2)
				   /	
				  /	
				 /
				/
target	(x1,y1) ---------- target's weapon
	*/

	angle1 = atan( (u->y - t->y) / (u->x - t->x) );

	if ( u->x - t->x < 0  )
		angle1 += mPI;
	else if ( u->y - t->y < 0  )
		angle1 += mPI*2;

	temp.x = u->x + cos(angle1+mPI/1000.0);
	temp.y = u->y + sin(angle1+mPI/1000.0);

	if ( dist( t->wep, temp ) < dist ( t->wep, (*u)) )
		return false; //counter clock direction
	else
		return true; // clock direction
}

////////////////////////////////
// Pre-Script
// common parts of the scripts located at the front
// 1. updates the AI timer
// 2. resets target when it is dead
///////////////////////////////
inline bool microAI::Common_PreScript(Unit *u){
	u->AI_timer++;
	if ( u->AI_timer > 0 ) 
		u->AI_timer %= AI_period;

	if ( u->AI_timer < AI_period-1 ) 
		return true;

	if ( u->Target->dead ) {
		u->AI_sequence = 0;
		u->AI_timer = AI_period-2;
		u->ACT_type = ACT_idle;
		return true;
	}
	return false;
}

////////////////////////////////
// Post-Script
// common parts of the scripts located at the end
//   handles cases when the unit is at the edge of the map
///////////////////////////////
inline void microAI::Common_PostScript(Unit *u, Unit *t){
	//Running out from the edge of map
	//if it already has an attack plan, that plan is carried out
	if ( 
		( u->ACT_type == ACT_guard ||
		    u->ACT_type == ACT_idle ) 
		&& (
		  dist( (*u), (*u->Lcenter) ) > BATTLE_RANGE/2
		  ) 	
		){
		//changes the rotation direction of unit movement
		//simple way to avoid the bump on the wall
		u->MV_clock_wise = !u->MV_clock_wise;

		if ( dist((*t), (*u)) > t->wep.WD->length*1.3) {
			//target is far away, move to center 
			u->MV_dest.x = u->Lcenter->x;
			u->MV_dest.y = u->Lcenter->y;
			u->MV_type = MV_to_P;
			Uguard
		}
		else {
			u->MV_type = MV_to_T;
			u->MV_dist = 0.0;
			u->ACT_type = ACT_poke;
		}
		u->AI_timer = AI_period-3;
	}

	if ( u->dist_to_closest_enemy > 1600 && u->MV_type == MV_to_T_diag ) {
		u->MV_type = MV_to_T;
		u->MV_dist = 100.0;
	}
}