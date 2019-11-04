#pragma once
#include "DataTypes.h"
#include <math.h>

/*

The AI class for micro control.

Mainly concerns the actions selected for the units based on 

1 vs 1 scenario 

*/
#define BusyCALL		u->AI_timer = AI_period-2; //immediately calls 

#define Tattack		( t->ACT_type == ACT_fswing || t->ACT_type == ACT_hswing || t->wep.on_poke)
#define TAattack	( t->wep.Wep_status == Wep_on_swing || t->wep.Wep_breaks_guard || t->wep.Wep_status == Wep_on_poke || t->wep.Wep_status == Wep_on_dns)
#define Tswing		( t->ACT_type == ACT_fswing || t->ACT_type == ACT_hswing )
#define Thswing		( t->ACT_type == ACT_hswing )
#define Tfswing		( t->ACT_type == ACT_fswing )
#define Tpoke		( t->wep.on_poke   )
#define Tguard		( t->wep.on_guard ) 

#define Tclose		(dist( (*u), (*t) ) <  t->wep.WD->length * 0.8 )
#define Tpokable	(dist( (*u), (*t) ) <  t->wep.WD->length * 3.0 )
#define Tfar		(dist( (*u), (*t) ) >  t->wep.WD->length * 1.1 )
#define Tdist		 dist( (*u), (*t) )
#define Gdist		 dist( (*u), (*u->T_second) )
#define TdistG		 dist( (*u->T_second), (*t) )

#define Uguard   u->ACT_type = ACT_guard;
#define Upoke    u->ACT_type = ACT_poke; u->AI_timer -= 2*AI_ACTION_period;
#define Ufswing  u->ACT_type = ACT_fswing; u->AI_timer -= 2*AI_ACTION_period;
#define Uhswing  u->ACT_type = ACT_hswing; u->AI_timer -= 2*AI_ACTION_period;
#define Uidle    u->ACT_type =  ACT_idle;
#define Udns     u->ACT_type =  ACT_dashNslash;
#define Uspin    u->ACT_type = ACT_spin; u->AI_timer -= 2*AI_ACTION_period;

#define AIDelay(i) u->AI_timer -= i*AI_ACTION_period;

class microAI
{
	double angle1, angle2;
	dPOINT temp;

	inline bool Common_PreScript(Unit *);
	inline void Common_PostScript(Unit *, Unit *);

public:
	microAI(void);
	~microAI(void);

	void march(Unit *);

	void guard_Sword(Unit *, Unit *); //guards the secondary target. 
	void simple_Sword(Unit *, Unit *);

	void simple_Dagger(Unit *, Unit *);

	void simple_Bow(Unit *, Unit *);

	bool circulate_direction(Unit*, Unit*);
};

