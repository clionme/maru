#pragma once

#include "DataTypes.h"

/*

A Field battle engine...

Handles LGroups and

 and 

processes LGroup-level battle field 

*/

class battleFEE
{
	dPOINT cp; //center of respawn point
	dPOINT dp; //center of destination point
public:
	battleFEE(void){};
	~battleFEE(void){};

	void FEE_Adjust_ef(LGroup *,LGroup *);
	void FEE_Update_LG_Status(LGroup *L, LGroup* AL);
	void FEE_Group_Position(SGroup*,Unit*,dPOINT,dPOINT,int);
	void FEE_Stepin(LGroup*);

	void SimpleMeta_MDead(LGroup*, int*);
	void SimpleMeta_MResetUnit(Unit*);
	void SimpleMeta_MRespawn(LGroup*);
	void SimpleMeta_MFormMarchSGroup(LGroup*,Unit*);
	void SimpleMeta_MRespawnSGroupPosition(SGroup*,Unit*);
};