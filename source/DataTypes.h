#pragma once
/*

Data types for YUI_AR

*/
#include <math.h>

//#define debug_mode

//#define smode //spirit mode

#define mPI 3.14159265

#define WINDOW_WIDTH 800  //Display Window Height - 800
#define WINDOW_HEIGHT 600 //Display Window Height - 600

#define WWAD 400 //= WINDOW_WIDTH / 2; // Window width adjust for display mode 2
#define WHAD 300 //= WINDOW_HEIGHT / 2; // Window height adjust for display mode 2

#define BATTLE_RANGE 6000.0

#define MAXARROW 400 //maximum number of arrows
#define WEP_BOW_0_RANGE 400 //range of BOW_0

#define RESPAWN_PERIOD 300
#define RESPAWN_PERIOD_D 300.0 //period in double form..
#define RESPAWN_PERIOD_NINETY_P 270.0 //90%
#define RESPAWN_PERIOD_TEN_P    30.0 //10%

#define EFFECT_NUM		100
#define EFFECT_DURATION 20
#define MAX_WEP_SPARKLE 15

#define dist(a,b) sqrt(pow((a.x - b.x),2) + pow((a.y - b.y),2))

#define AI_period 15 // AI is updated on every 15th call :: 
					//smaller : more frequent update, but not necessarily better on the power of AI
//make it over 15 : update on every 15*3 = 45 pixel movement,  character is 48 pixel.(3 : speed)

#define AI_ACTION_period 60 // Delay unit for each action

#define NON_PULLED_WEP_U   u->wep.weapon_type != 3
#define NON_PULLED_WEP_OU  ou->wep.weapon_type != 3
#define NON_PULLED_WEP_UA   u[i].wep.weapon_type != 3

#define MAX_NUM_TEAM 2
#define MAX_NUM_LGROUP 20
#define MAX_NUM_SGROUP 36 // estimates 2 / group[0] (this setting may cause seg-fault)
#define NOT_IN_SGROUP -1
#define MAX_NUM_UNIT 100
						
//weapon's end point is places at (+8, +12) away from the 
//center of an unit for aesthetics.
//i.e., the center of an unit is its lefthad at ( +8,+12 )	
#define unit_position_adj_x 8  //unit position adjustment, x-axis
#define unit_position_adj_y 12 //unit position adjustment, y-axis

#define ZOOM_M_THRESHOLD 100 
#define ZOOM_L_THRESHOLD 10

#define NUM_HAT 10

enum Enum_MV_Type { //Move types
		MV_userunit = 0,
		MV_to_T		= 1,
		MV_dist_T	= 2,

		//MV_to_T_zig = 4,
		MV_cir_T	= 5,
		//MV_cir_T_c	= 6, //counter clock direction

		MV_to_T_diag = 7,
		MV_away_T_diag = 8,

		MV_to_T_g   = 9, //guards secondary target against primary target

		MV_to_P			= 31,
		MV_cir_P		= 32,
		MV_to_P_march	= 33,

		MV_death_ani	= 41, //death animation. 
};

enum Enum_LGroup_Status { //SGroup Action types
	LS_idle = 0,
	LS_inbattle = 1,
	LS_flee = 2,
	LS_march = 3,
};

enum Enum_SGroup_ACT_Type { //SGroup Action types
	GACT_idle = 0,
	GACT_GetArcher = 1, //can implement a action to attack a single archer as a group[0]
	GACT_HoldPosition = 2,

	//GACT_Stay_SGrouped = 3,
	GACT_Stay_Formation = 4,
	GACT_Stay_Close = 5,

	GACT_GuardArcher = 6,
};

enum Enum_ACT_Type { //ACTION types
	ACT_idle = 0,

	ACT_swing = 1,
	ACT_poke  = 2,
	ACT_guard = 3,

	ACT_hswing = 4, //half swing
	ACT_fswing = 5, //full swing

	ACT_spin = 6,

	ACT_dashNslash = 9,

	ACT_bowaim = 11,
	ACT_bowfire = 12,
};

enum Enum_Wep_Status { // Weapon status .
	Wep_on_idle	   = -1,
	Wep_on_guard   = 0,
	Wep_on_poke    = 1,
	Wep_on_swing   = 2,
	Wep_on_dns     = 3, //dash n slash
	//Wep_on_fswing  = 3,
	//Wep_on_hswing  = 4,
};

enum Enum_HitType{ //Hit types
	Hit_GuardBreak = 1,
	Hit_Normal     = 2,
	Hit_Guarded    = 3,
	Hit_DnSB	   = 14,
	Hit_DnSG	   = 15,
};

#define NUM_UNIT_TYPES 6 // +1 for counting all units ( Enum_Unit_Type -> Unit_all_unit ) 
						 // some functions depend on this +1 here. => Always have this +1 added <=

enum Enum_Unit_Type{
	Unit_all	 = 0,
	Unit_sword   = 1,
	Unit_gsword  = 5,
	Unit_dagger  = 4,
	Unit_bow     = 3,
	Unit_spear   = 2,
};

struct dPOINT{
	double x;
	double y;
};

struct HitEffect{
	dPOINT pos;
	int timer;
	int type;
};

struct Arrow{
	int team; 

	dPOINT back, front;
	double speed;
	double counter;

	bool flying;
	bool active;
	bool onbow;

	double hwidth;
	double length;

	int arrow_type;
	double collision_radius;

	dPOINT c; //collision point
};

 struct WeaponData{
	//Size
	double hwidth; //half width
	double length;

	 //Pull & Poke 
	double		pull_max;
	double		pull_speed;
	double		poke_speed;
	double		poke_move_distance;

	//Speed limits
	double		max_angle_speed;
	double		max_plane_speed;

	//ETC
	int edges; //number of points which is counted as edges, used in collision detection
} ;

struct Weapon{

	WeaponData *WD;

	double x, y;
	dPOINT old;
	dPOINT c[6]; //collision points 0,1,2,.. ,4 for weapon, 5 for dash mode

	double angle;
	double target_angle;
	int weapon_type; // 1 : sword
					 // 2 : spear
					 // 3 : bow
					 // 4:  dagger

	double angle_speed; 
	double angle_accel;

	double pulled;//  for spear weapon, tells you how much the weapon is pulled back
	dPOINT pulled_p; // end point of pulled pole

	bool on_hit;

	int Wep_status;  //uses Enum_Wep_Status
	bool Wep_breaks_guard;

	bool direction; // true : clock wise, false : counter clock wise

	int stance; // 0 : guard, move mode 1: accel 2: bcel
	int next_stance; // next stance
	bool stance_shift;
	int stance_charge;
	int num_of_stance;

	dPOINT skp[MAX_WEP_SPARKLE];
	int skp_counter;
} ;

struct Unit{
	double x, y; //position
	dPOINT old;
	dPOINT skill; // target position where skill is used 
	int direction;
	int sequence;

	bool userunit;
	bool active;     //whether the unit is being used
	bool dead;       //dead or alive

	int DA_counter;  //Dead Animation

	int stat_maxHP;
	int stat_maxST;
	int stat_maxMP;
	double stat_currentHP;
	double stat_currentST;
	double stat_currentMP;
	int stat_recover_speed;

	int status_slow; // on : status_slow > 0  off : status_slow < 0

	double stamina;

	double speed;
	double max_speed;

	double dist_to_closest_enemy;

	bool hit;
	bool roll;
	bool on_screen;

	int team; 
	int group[2];// [0] : small level within the battle field 
				 // [1] : large level for the larger scale battle field

	int AI_timer;      //order is given based on this timer
	int AI_var_wait;   //variable for busy-wait type of design.
	int AI_sequence;   //AI action sequence number
	int AI_preset[3];  //AI presets
					   //[0] : micro level (1 vs 1)
					   //[1] : group[0] level (n vs n, n < 4 ) n : party of units
					   //[2] : macro level (N vs N)		    N : group[0] of parties


	int TargetedBy;	 // counts how many MELEE units are targetting this unit.
					 // userunit is counted to target all the units near by
	
	int TargetedBy_R;// counts how many RANGED units are targetting this unit.
					 // userunit is counted to target all the units near by

	int ACT_type; //uses Enum_ACT_Type 

	dPOINT *Lcenter;
	dPOINT MV_dest; // destination
	int MV_type;
	double MV_dist;
	bool MV_clock_wise;

	dPOINT difM;
	Weapon wep;
	Arrow *arwp;

	Unit * Target;
	Unit * T_second; // Secondary Target used for allocating guard mode.
};



/////////////////////////////////
//Small sized Group
//this is the collection of units
class SGroup{ 

	int num_member;
	int position_sequence[NUM_UNIT_TYPES];
	int id;

public:
	int team;	
	int unit_type; //( -1 for mixed group[0], otherwise uses enum Unit type )
	dPOINT MV_Gdestination;
	double speed; //speed of the group[0]
	dPOINT center; //center of the group[0]

	Unit *GTarget; // target of the group[0]
	double GTarget_dist;   //distance to the group[0]'s target
	Unit *member[MAX_NUM_UNIT+1];

	void reset(int id_in){
		this->id = id_in;
		this->num_member = 0;
	}

	void add(Unit * u){
		this->member[this->num_member] = u;

		this->num_member++;
		u->group[0] = this->id;
		
		this->team = u->team; 
		//this part can cause error
		this->unit_type = u->wep.weapon_type; 

		if ( this->num_member != 0 ) {
			center.x = (u->x + (this->num_member-1)*center.x) / (this->num_member) ; 
			center.y = (u->y + (this->num_member-1)*center.y) / (this->num_member) ; 
		}
	}

	void remove(Unit * u){
		//find u
		for ( int i = 0 ; i < num_member; i++ ) {
			if ( this->member[i] == u ) {
				this->member[i] = this->member[num_member-1];
			}
		}

		this->num_member--;
		u->group[0] = NOT_IN_SGROUP;

		if ( this->num_member > 0 ) {
			center.x = ((this->num_member+1)*center.x - u->x) / (this->num_member) ; 
			center.y = ((this->num_member+1)*center.y - u->y) / (this->num_member) ; 
		}
	}

	void position_remove(Unit *u) {
		this->position_sequence[u->wep.weapon_type]--;	
	}

	void position_add(Unit * u) {
		this->position_sequence[u->wep.weapon_type]++;	
	}

	void reset(){
		this->num_member = 0;
		for ( int i = 0 ; i < NUM_UNIT_TYPES; i++ )
			this->position_sequence[i] = 0;	
	}

	int position_sequence_of(int t){
		if ( t >=0 && t < NUM_UNIT_TYPES )
			return this->position_sequence[t];	
		else
			return -1;
	}

	bool isNempty(){
		return num_member != 0;
	}

	bool hasUnit(Unit *u){
		return u->group[0] == this->id;
	}

	int print_num_member(){
		return this->num_member;
	}
};


///////////////////////////////
//Large sized Group
//this is the collection of groups and units
class LGroup{
	/////////////////////////////
	//LGroup Selector
	/////////////////////////////
	LGroup * f_LG[MAX_NUM_LGROUP];
	LGroup * e_LG[MAX_NUM_LGROUP];
	int f_num; //number of friendly LGs near by;
	int e_num; //number of enemy    LGs bear by;
	int id;

public:

	bool user_order_given;

	int Status;
	dPOINT boundary[4];
	int team;
	bool usergroup;
	bool active;
	bool within_battle_range;
	bool empty;
	Unit uDefaultDestination;
	dPOINT center;
	double num_member;

	Unit u[MAX_NUM_UNIT+1];
	SGroup g[MAX_NUM_SGROUP];
	//SGroup *g;

	///////////////////////////
	LGroup(){ //... T_T	
	};
	
	~LGroup(){
	}
	////////////////////////////

	void LG_reset(int my_id){
		id = my_id;
		team  = my_id%MAX_NUM_TEAM;
		active = false;
		within_battle_range = false;
		usergroup = false;

		Status = LS_idle;

		SG_reset();

		for ( int i = 0 ; i <= MAX_NUM_UNIT; i++) {
			u[i].team = this->team;
			u[i].group[1] = my_id;
			u[i].Lcenter = &this->center;

			u[i].active = false;
			u[i].dead = false;
			u[i].Target = NULL;
		}
		user_order_given = false;
		Sel_reset();
	}

	void SG_reset(){
		for ( int i = 0; i < MAX_NUM_SGROUP; i++ ) {
			g[i].reset(i);
			g[i].team = this->team;
		}
	}

	void calc_nmember(){
		num_member = 0.0;
		for ( int i = 0; i <= MAX_NUM_UNIT; i++ ){
			if ( u[i].active && !u[i].dead ) {
				num_member += 1.0;
				empty = false;
			}
		}
		if ( num_member < 0.1 ) {
			//active = false;
			empty = true;
		}
	}

	void calc_center(){
		if ( !this->within_battle_range )
			return;

		calc_nmember();

		if ( num_member < 0.1 ) {
			Sel_reset();
			center.x = u[MAX_NUM_UNIT].x;
			center.y = u[MAX_NUM_UNIT].y;
			return;
		}
		center.x = 0.0;
		center.y = 0.0;
		for ( int i = 0; i <= MAX_NUM_UNIT; i++ ){
			if ( u[i].active && !u[i].dead ) {
				center.x += u[i].x;
				center.y += u[i].y;
			}
		}

		//active = true;
		center.x /= num_member;
		center.y /= num_member;	
	}

	void Set_boundary(){
		if ( !this->within_battle_range )
			return;

		boundary[0].x = center.x; boundary[0].y = center.y;
		boundary[1].x = center.x; boundary[1].y = center.y;
		boundary[2].x = center.x; boundary[2].y = center.y;
		boundary[3].x = center.x; boundary[3].y = center.y;

		/*

		[0]    [1]


		[2]    [3]

		*/

		for ( int i = 0 ; i <= MAX_NUM_UNIT; i++ ) {
			if ( u[i].active && !u[i].dead ) {
				if ( u[i].x < boundary[0].x  ) {
					boundary[0].x = u[i].x;
					boundary[2].x = u[i].x;
				}
				else if ( u[i].x > boundary[1].x  ) {
					boundary[1].x = u[i].x;
					boundary[3].x = u[i].x;
				}

				if ( u[i].y < boundary[0].y  ) {
					boundary[0].y = u[i].y;
					boundary[1].y = u[i].y;
				}
				else if ( u[i].y > boundary[2].y  ) {
					boundary[2].y = u[i].y;
					boundary[3].y = u[i].y;
				}
			}
		}

		boundary[0].x -= 1000.0;  boundary[0].y -= 1000.0;
		boundary[1].x += 1000.0;  boundary[1].y -= 1000.0;
		boundary[2].x -= 1000.0;  boundary[2].y += 1000.0;
		boundary[3].x += 1000.0;  boundary[3].y += 1000.0;
	}

	void Sel_reset(){
		this->f_LG[0] = this;
		this->f_num = 1;
		this->e_num = 0;
	};

	int print_e_num(void){
		return this->e_num;
	}
	int print_f_num(void){
		return this->f_num;
	}
	LGroup* e(int i){
		return this->e_LG[i];
	}
	LGroup* f(int i){
		return this->f_LG[i];
	}

	///////
	//f_LG
	bool Sel_add_f(LGroup *L){
		if ( f_num < MAX_NUM_LGROUP-1 ) {
			for ( int i = 0 ; i < f_num; i++ ){
				if ( f_LG[i]->id == L->id ) {
					return false; //we already have the LGroup
				}
			}
			f_LG[f_num] = L;
			f_num++;
			return true;
		}
		else
			return false;
	}
	bool Sel_remove_f(LGroup *L){
		for ( int i = 1 ; i < f_num; i++ ){
			if ( f_LG[i]->id == L->id ) {
				f_num--;
				f_LG[i] = f_LG[f_num];
				return true;
			}
		}
		return false;
	}

	///////
	//e_LG

	// e_LG[0] is used as the cloest LGroup, 
	// check 'battleFEE::FEE_Update_LG_Status' before making any changes to this part
	bool Sel_add_e(LGroup *L){
		if ( e_num < MAX_NUM_LGROUP-1 ) {
			for ( int i = 0 ; i < e_num; i++ ){
				if ( e_LG[i]->id == L->id ) {
					return false; //we already have the LGroup
				}
			}
			e_LG[e_num] = L;
			e_num++;
			return true;
		}
		else
			return false;
	}
	bool Sel_remove_e(LGroup *L){
		for ( int i = 0 ; i < e_num; i++ ){
			if ( e_LG[i]->id == L->id ) {
				e_num--;
				e_LG[i] = e_LG[e_num];
				return true;
			}
		}
		return false;
	}

	void Sel_set_order_e(){
		LGroup *dummy_L;
		for ( int i = 1; i < e_num; i++ ) {
			if ( dist(e_LG[i]->center, this->center) < dist(e_LG[0]->center, this->center) ) {
				dummy_L = e_LG[0];
				e_LG[0] = e_LG[i];
				e_LG[i] = dummy_L;
				i = 1;
			}
		}
	}

};


