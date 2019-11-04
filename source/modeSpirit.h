#pragma once

#include "DataTypes.h"
#include <math.h>
/*

Da spirit mode >_<

*/

#define cur (*p_cur)

class modeSpirit
{
	LGroup* mL;
	POINT *p_cur;

	int charge;
public:
	bool mode_on;
	int team;

	modeSpirit(POINT*);
	modeSpirit(void);
	~modeSpirit(void);

	int print_charge(){
		return this->charge;
	}
	void reset();
	void Spirit_Main(LGroup*);

};

