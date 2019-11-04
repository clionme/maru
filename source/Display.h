#pragma once

/*

Display class, 

draws objects on to the back_dc, the back buffer

*/

#include "DataTypes.h"
#include <math.h>

#ifdef smode
#include "modeSpirit.h"
#endif

#define DISP_HP_BAR_Y_BOT 21
#define DISP_HP_BAR_Y_TOP 24

#define MAX_CURSOR_SPARKLE 20
#define PRINT_ICON ( (*zoom) < 100 )
#define PRINT_LICON ( (*zoom) < 10 )
#define DRAW_U  (u[i].active && !u[i].dead && u[i].on_screen)

#define DRAW_ICON_COOR_X(i) (i - disp_center->x)*(*zoom)/100.0 + WWAD
#define DRAW_ICON_COOR_Y(i) (i - disp_center->y)*(*zoom)/100.0 + WHAD

#define DISP_ADJ_X (disp_center->x - WWAD)
#define DISP_ADJ_Y (disp_center->y - WHAD)

#define zoomed (100.0/(*zoom))

class Display
{

	RECT back_rect;
	POINT pntArray[5];
	POINT pt_weapon[3];

	HDC background_dc;
	HBITMAP background_bmp;

	HDC background_tile_dc[5];
	HBITMAP background_tile_bmp[5];

	HDC titlelogo_dc;
	HBITMAP titlelogo_bmp;

	HDC button_dc[5];
	HBITMAP button_bmp[5];
	HDC button_m_dc[5];
	HBITMAP button_m_bmp[5];

	HDC back_dc, backS_dc, //temporary dc
		arrow_dc, 

		betty_dc, betty_m_dc, betty_m2_dc,
		george_dc, george_m_dc, 
		
		shield_dc, shield_m_dc,
		status_dc, status_m_dc,
		
		wep_mini_dc[NUM_UNIT_TYPES],
		wep_dc[NUM_UNIT_TYPES],
		
		hat_dc[NUM_HAT],
		hat_m_dc[NUM_HAT];

	HBITMAP back_bmp;
	HBITMAP backS_bmp;
	HBITMAP old_bmp;
	
	HBITMAP betty_bmp, betty_m_bmp, betty_m2_bmp;
	HBITMAP george_bmp, george_m_bmp;

	HBITMAP arrow_bmp;

	HBITMAP shield_bmp;
	HBITMAP shield_mask_bmp; 

	HBITMAP status_bmp;
	HBITMAP status_mask_bmp;

	HBITMAP wep_mini_bmp[NUM_UNIT_TYPES];
	HBITMAP wep_bmp[NUM_UNIT_TYPES];
	HBITMAP wepm_bmp[NUM_UNIT_TYPES];

	HBITMAP hat_bmp[NUM_HAT];
	HBITMAP hatm_bmp[NUM_HAT];

	HBITMAP arrow_mask_bmp; 

	HBRUSH SparkleP[MAX_WEP_SPARKLE]; 
	HBRUSH SparkleG[MAX_WEP_SPARKLE]; 

	HBRUSH Sparkle2[MAX_CURSOR_SPARKLE]; 

	//variables for rotated weapon sprite
	double rotate_R ;
	double rotate_dy ;
	double rotate_dx ;

	//variables for storing pre-pulled axis
	double temp_ux;
	double temp_uy;

	//variables for weapon reach display
	dPOINT wepr;
	bool wepr_Button_was_down;
	bool wepr_clock;
	bool wepr_aclock;

	TCHAR Disp_text[200];

	int cursoreffect_x[MAX_CURSOR_SPARKLE];
	int cursoreffect_y[MAX_CURSOR_SPARKLE];
	int cursoreffect_counter;
	
	// circling sparkles around the 'LIVE' button
	int buttoneffect1_x[MAX_WEP_SPARKLE];
	int buttoneffect1_y[MAX_WEP_SPARKLE];
	int buttoneffect1_counter;

	int buttoneffect2_x[MAX_WEP_SPARKLE];
	int buttoneffect2_y[MAX_WEP_SPARKLE];
	int buttoneffect2_counter;

	int NUM_SPARKLE;
	HBRUSH *SparkleVar;
	int *Sparkle_counter;
	double circle_size;
	double max_circle_size;

	POINT pt_buttoneffect;

	int *ceffect_x, *ceffect_y;

	int boundarySize;

	Arrow *disp_arw;
	HitEffect *disp_hef;

	int dummy_dx, dummy_dy;
	int minimap_resolution;
	double *zoom; //zoom, x : x percent. 100 -> 1:1 from the bmp
	POINT *disp_center; // center of the screen
	dPOINT dummy;

	inline void DrawBGGridLine(int);
	inline void DrawBGGridLineIcon(int);

	inline void DrawUnit_Mask(Unit *);
	inline void DrawUnit_Unit(Unit *);
	inline void DrawUnit_Hat(Unit *);
	inline void DrawUnit_Peri(Unit *);

	inline void DrawUnit_ICON(Unit *);
	inline void DrawUnit_LICON(LGroup *);

	inline void DrawWeapon( Unit*);
	inline void DrawWepSparkles(Unit *);
	inline void DrawArrow();
	inline void DrawWepHiteffect();

public:
	Display(HDC, HINSTANCE, double*, POINT *,  Arrow*, HitEffect*);
	Display(void){};
	~Display(void);

	void DrawUnitsToDraw(LGroup* );

	void DrawField( LGroup* );

	void DrawStaminabar(Unit*);
	
	void DrawRESET_back_dc();
	void DrawBackground();

	void DrawText(TCHAR*, int, int);
	void DrawText(TCHAR*, int, int, HDC);

	void DrawToForeDC(HDC);


	void DrawCursor(POINT, int);
	void DrawWepReach(Unit *, POINT, bool, bool);

	void DrawButton(bool,int);

	void DrawTitleLogo(POINT);

	void DrawMiniMap(Unit *);

	#ifdef debug_mode
		LGroup *Maru_L; //used only for debugging purposes
	#endif

	#ifdef smode
		void DrawSmode( modeSpirit* );
	#endif
};




