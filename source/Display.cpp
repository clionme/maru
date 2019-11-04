#include "StdAfx.h"
#include "Display.h"
#include <stdio.h>
Display::Display(HDC fore_dc, HINSTANCE hInst, double *zoom_in, 
				POINT * center_in, Arrow *arw_in, HitEffect *hef_in)
{
	disp_center = center_in;
	
	zoom = zoom_in;

	disp_arw = arw_in;
	disp_hef = hef_in;

	back_rect.left = 0;				back_rect.top = 0;
	back_rect.right = WINDOW_WIDTH;
	back_rect.bottom = WINDOW_HEIGHT;

	back_dc = CreateCompatibleDC(fore_dc);
	back_bmp = CreateCompatibleBitmap(fore_dc,WINDOW_WIDTH,WINDOW_HEIGHT);
	SelectObject(back_dc, back_bmp);

	backS_dc = CreateCompatibleDC(fore_dc);
	backS_bmp = CreateCompatibleBitmap(fore_dc,WINDOW_WIDTH,WINDOW_HEIGHT);
	SelectObject(backS_dc, backS_bmp);
/*
	background_dc = CreateCompatibleDC(fore_dc);
	background_bmp = (HBITMAP)LoadImage(hInst, L".\\bmp.\\bg_2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(background_dc, background_bmp);
*/
	background_tile_dc[0] = CreateCompatibleDC(fore_dc);
	background_tile_bmp[0] = (HBITMAP)LoadImage(hInst, L".\\bmp.\\bg_3.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(background_tile_dc[0], background_tile_bmp[0]);

	titlelogo_dc  = CreateCompatibleDC(fore_dc);
	titlelogo_bmp = (HBITMAP)LoadImage(hInst, L".\\bmp.\\TitleS.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(titlelogo_dc,titlelogo_bmp);

	betty_bmp     = (HBITMAP)LoadImage(hInst, L".\\bmp.\\betty_1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	betty_m_bmp   = (HBITMAP)LoadImage(hInst, L".\\bmp.\\betty_1m.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	betty_dc = CreateCompatibleDC(fore_dc);
	betty_m_dc = CreateCompatibleDC(fore_dc);
	SelectObject(betty_dc,betty_bmp);
	SelectObject(betty_m_dc,betty_m_bmp);

	george_bmp    = (HBITMAP)LoadImage(hInst, L".\\bmp.\\george_0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	george_m_bmp  = (HBITMAP)LoadImage(hInst, L".\\bmp.\\george_0m.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	george_dc = CreateCompatibleDC(fore_dc);
	george_m_dc = CreateCompatibleDC(fore_dc);
	SelectObject(george_dc,george_bmp);
	SelectObject(george_m_dc,george_m_bmp);

	shield_bmp       = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_shield1.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	shield_mask_bmp  = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_shield1m.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	shield_dc = CreateCompatibleDC(fore_dc);
	shield_m_dc = CreateCompatibleDC(fore_dc);
	SelectObject(shield_dc,shield_bmp);
	SelectObject(shield_m_dc,shield_mask_bmp);

	status_bmp       = (HBITMAP)LoadImage(hInst, L".\\bmp.\\s_slow_small.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	status_mask_bmp  = (HBITMAP)LoadImage(hInst, L".\\bmp.\\s_slow_smallm.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	status_dc = CreateCompatibleDC(fore_dc);
	status_m_dc = CreateCompatibleDC(fore_dc);
	SelectObject(status_dc,status_bmp);
	SelectObject(status_m_dc,status_mask_bmp);

	arrow_bmp       = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_arrow2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	arrow_mask_bmp  = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_arrow2m.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	arrow_dc = CreateCompatibleDC(fore_dc);
	SelectObject(arrow_dc,arrow_bmp);

	for ( int i = 0 ; i < NUM_UNIT_TYPES; i++ ) {
		wep_mini_dc[i] = CreateCompatibleDC(fore_dc);
		wep_dc[i] = CreateCompatibleDC(fore_dc);
		switch(i){
		case 0: //there is always +1 to 'NUM_UNIT_TYPES'
			break;
		case 1:
			wep_mini_bmp[i] = (HBITMAP)LoadImage(hInst, L".\\bmp.\\Mini_Sword0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			wep_bmp[i]      = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_sword0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			wepm_bmp[i]     = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_sword0m.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			break;
		case 2:
			wep_mini_bmp[i] = (HBITMAP)LoadImage(hInst, L".\\bmp.\\Mini_Spear0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			wep_bmp[i]      = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_spear0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			wepm_bmp[i]     = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_spear0m.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			break;
		case 3:
			wep_mini_bmp[i] = (HBITMAP)LoadImage(hInst, L".\\bmp.\\Mini_Bow0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			wep_bmp[i]      = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_bow0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			wepm_bmp[i]     = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_bow0m.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			break;
		case 4:
			wep_mini_bmp[i] = (HBITMAP)LoadImage(hInst, L".\\bmp.\\Mini_Dagger0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			wep_bmp[i]      = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_dagger0.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			wepm_bmp[i]     = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_dagger0m.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			break;
		case 5:
			wep_mini_bmp[i] = (HBITMAP)LoadImage(hInst, L".\\bmp.\\Mini_Sword2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			wep_bmp[i]      = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_sword2.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			wepm_bmp[i]     = (HBITMAP)LoadImage(hInst, L".\\bmp.\\w_sword2m.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
			break;
		}
		SelectObject(wep_mini_dc[i],wep_mini_bmp[i]);
		SelectObject(wep_dc[i],wep_bmp[i]);
	}


	for ( int i = 0 ;  i < NUM_HAT; i++ ) {
		hat_dc[i] = CreateCompatibleDC(fore_dc);
		hat_m_dc[i] = CreateCompatibleDC(fore_dc);

		_stprintf_s(Disp_text, _T(".\\bmp.\\hat_0%d.bmp"),i);	
		hat_bmp[i]      = (HBITMAP)LoadImage(hInst, Disp_text, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		_stprintf_s(Disp_text, _T(".\\bmp.\\hat_0%dm.bmp"),i);	
		hatm_bmp[i]     = (HBITMAP)LoadImage(hInst, Disp_text, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		SelectObject(hat_dc[i],hat_bmp[i]);
		SelectObject(hat_m_dc[i],hatm_bmp[i]);
	}

	for ( int i = 0 ;  i < 5; i++ ) {
		button_dc[i] = CreateCompatibleDC(fore_dc);
		button_m_dc[i] = CreateCompatibleDC(fore_dc);

		_stprintf_s(Disp_text, _T(".\\bmp.\\button_%d.bmp"),i);	
		button_bmp[i]      = (HBITMAP)LoadImage(hInst, Disp_text, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		_stprintf_s(Disp_text, _T(".\\bmp.\\button_%dm.bmp"),i);	
		button_m_bmp[i]      = (HBITMAP)LoadImage(hInst, Disp_text, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

		SelectObject(button_dc[i],button_bmp[i]);
		SelectObject(button_m_dc[i],button_m_bmp[i]);
	}

	for ( int i = 0 ; i < MAX_WEP_SPARKLE; i++ ) {
		//purple
		//SparkleP[i] = CreateSolidBrush(RGB( 255, 20, 65+100/MAX_WEP_SPARKLE*i));
		//yellow
		SparkleP[i] = CreateSolidBrush(RGB( 65+190/MAX_WEP_SPARKLE*i, 238, 20));
		//blue
		SparkleG[i] = CreateSolidBrush(RGB( 0, 50+200/MAX_WEP_SPARKLE*i, 255));

		//use this one when using 40 particles
		//SparkleG[i] = CreateSolidBrush(RGB( 0, 50+5*i, 255));
	}

	for ( int i = 0 ; i < MAX_CURSOR_SPARKLE; i++ ) {
		Sparkle2[i] = CreateSolidBrush(RGB( 40+10*i, 40+10*i, 40+10*i));
	}
	cursoreffect_counter = 0;
	buttoneffect1_counter= 0;
	buttoneffect2_counter= 0;
	boundarySize = 10;
}


Display::~Display(void)
{
	//free(back_dc);
}

//Colors
HBRUSH yellow_brush = CreateSolidBrush(RGB(255,255,0));
HBRUSH blue_brush = CreateSolidBrush(RGB(0,0,255));
HBRUSH red_brush = CreateSolidBrush(RGB(255,0,0));
HBRUSH orange_brush = CreateSolidBrush(RGB(255,150,0));
HBRUSH lightgreen_brush = CreateSolidBrush(RGB(150,250,0));
HBRUSH darkgreen_brush = CreateSolidBrush(RGB(0,200,0));

HBRUSH redorange_brush = CreateSolidBrush(RGB(255,211,211)); 
//HBRUSH redorange_brush = CreateSolidBrush(RGB(204,17,0)); 
//http://desktoppub.about.com/od/choosingcolors/f/what-color-is-blood-red.htm

HBRUSH white_brush  = CreateSolidBrush(RGB(255,255,255));
HBRUSH black_brush  = CreateSolidBrush(RGB(0,0,0));
HBRUSH grey_brush  = CreateSolidBrush(RGB(40,40,0));
HBRUSH green_brush  = CreateSolidBrush(RGB(0,186,70));
HBRUSH green2_brush = CreateSolidBrush(RGB(241,250,255));

HBRUSH redB_brush     = CreateSolidBrush(RGB(120,	120,	120));
HBRUSH greenB_brush   = CreateSolidBrush(RGB(210,	210,	210));
HBRUSH yellowB_brush  = CreateSolidBrush(RGB(255,	255,	255));

HPEN nullPen = CreatePen(PS_NULL, 1, 0);
HPEN blackPen = CreatePen(PS_SOLID, 3, 0);
HPEN bluePen = CreatePen(PS_SOLID, 10, RGB(0,120,255));

HPEN white1Pen = CreatePen(PS_DOT, 1, RGB(190,0,0));
HPEN blue2Pen = CreatePen(PS_DOT,   1,  RGB(0,60,150));
HPEN blue3Pen = CreatePen(PS_SOLID, 4,  RGB(0,60,150));
HPEN blue3iPen = CreatePen(PS_SOLID, 2, RGB(0,60,150));
HPEN blue4Pen = CreatePen(PS_SOLID, 16,  RGB(0,80,170));
HPEN blue4iPen = CreatePen(PS_SOLID, 8,  RGB(0,80,170));
HPEN blue5Pen = CreatePen(PS_SOLID, 32,   RGB(0,120,255)); //RGB(0,0,80)
HPEN blue5iPen = CreatePen(PS_SOLID, 16,  RGB(0,120,255));
HPEN blue5iiPen = CreatePen(PS_SOLID, 2,  RGB(0,120,255));

HPEN yellow_dot_Pen = CreatePen(PS_DOT, 1, RGB(255,255,90));
HPEN yellow_solid_Pen = CreatePen(PS_SOLID, 1, RGB(255,255,90));
HPEN blue_dot_Pen = CreatePen(PS_DOT, 1, RGB(50,255,255));
HPEN blue_solid_Pen = CreatePen(PS_SOLID, 1, RGB(50,255,255));

HPEN redPen = CreatePen(PS_SOLID, 2, RGB(253,64,36));
HPEN greyPen = CreatePen(PS_DOT, 1, RGB(100,100,100));
HPEN green3Pen = CreatePen(PS_DOT, 1, RGB(0,128,255));
HPEN orangePen = CreatePen(PS_DOT, 1, RGB(255,128,0));
HPEN smallbluePen = CreatePen(PS_SOLID, 1, RGB(198,255,240));

///////////////////////
// Draws to fore_dc (( the one being printed on screen ))
//////////////////////
void Display::DrawToForeDC(HDC fore_dc){
		BitBlt(fore_dc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, back_dc, 0, 0, SRCCOPY);	
}

/////////////////////
// Resets back_dc ( the back buffer is turned into black ) 
/////////////////////
void Display::DrawRESET_back_dc(){
	//SetBkColor(back_dc, RGB(0,0,0)); // black background

	//if (PRINT_ICON)
	//	FillRect(backM_dc, &back_rect, black_brush);
	//else

	FillRect(back_dc, &back_rect, black_brush);

	// Method 1.
	FillRect(backS_dc, &back_rect, black_brush);
	 
	// Method 2.
/*	
	BitBlt(backS_dc, 0,0, WINDOW_WIDTH, WINDOW_HEIGHT, background_dc, 
		disp_center->x%180 + (disp_center->x < 0 ? 180 : 0 ), 
		disp_center->y%180 + (disp_center->y < 0 ? 180 : 0 ), SRCCOPY);
*/

	// Method 3.
#ifndef debug_mode
	//using this as a variable
	pntArray[2].x = 180;

	for ( int i = disp_center->x / pntArray[2].x - WINDOW_WIDTH/pntArray[2].x; 
				i < disp_center->x / pntArray[2].x + WINDOW_WIDTH/pntArray[2].x; i++ ) {
		
		pntArray[0].x = pntArray[2].x*i-DISP_ADJ_X;
		if ( pntArray[0].x < -pntArray[2].x || pntArray[0].x > WINDOW_WIDTH +pntArray[2].x )
			continue;
			
		for ( int i = disp_center->y / pntArray[2].x - WINDOW_HEIGHT/pntArray[2].x; 
					i < disp_center->y / pntArray[2].x + WINDOW_HEIGHT/pntArray[2].x; i++ ) {
				
			pntArray[0].y =  pntArray[2].x*i-DISP_ADJ_Y;	
			if ( pntArray[0].y < -pntArray[2].x || pntArray[0].x > WINDOW_HEIGHT + pntArray[2].x)
				continue;		

			BitBlt(backS_dc, pntArray[0].x, pntArray[0].y, 
				pntArray[2].x, pntArray[2].x, background_tile_dc[0], 
				0, 0, SRCCOPY);
		}	
	}
#endif

}

///////////////////
// Text output on (x,y) location
///////////////////
void Display::DrawText(TCHAR* text, int x, int y) {
	SetTextColor(back_dc, RGB(255,255,255));  // white text color
    SetBkColor(back_dc, RGB(0,0,0)); // black background
	TextOut(back_dc, x, y, text, _tcslen(text));
}
void Display::DrawText(TCHAR* text, int x, int y, HDC dc) {
	SetTextColor(dc, RGB(255,255,255));  // white text color
    SetBkColor(dc, RGB(0,0,0)); // black background
	TextOut(dc, x, y, text, _tcslen(text));
}

//////////////////////////
// Draw_Stamina bar
//////////////////////////
void Display::DrawStaminabar(Unit *A){
	/*
	int WCx, WCy;
	WCx = A->x - 400; 
	WCy = A->y - 300;

	POINT pntArray[2];

	pntArray[0].x=WCx+220;
	pntArray[0].y=WCy+50;
	pntArray[1].x=WCx+220;
	pntArray[1].y=WCy+75;

	SelectObject(back_dc, white_brush); // background
	Rectangle(back_dc,WCx+100,WCy+50, WCx+500,WCy+75);

	SelectObject(back_dc, yellow_brush); // the stamina bar
	Rectangle(back_dc,WCx+100,WCy+50, WCx+100.0+ A->stamina*4.0,WCy+75);

	SelectObject(back_dc, blackPen);
	Polyline(back_dc, pntArray, 2);
	*/	
}


/////////////////////////
// Draw_weapon
////////////////////////
inline void Display::DrawWeapon( Unit *u ){

	//we restore these values at the end of this function
	//it is done this way as it is easier to read !
	temp_ux = u->x;
	temp_uy = u->y;
	
	if ( u->wep.weapon_type != 3 ) {
		//changes for pulled weapon sprite
		u->x = u->wep.pulled_p.x;
		u->y = u->wep.pulled_p.y;
	}
	
	//Avoid division by zero
	if ( u->wep.x == u->x ) 
		u->wep.x -= 0.001;
	else if (u->wep.y == u->y)
		u->wep.y -= 0.001;
	
	rotate_R  = (u->wep.y - u->y) / (u->wep.x - u->x);
	rotate_dy = u->wep.WD->hwidth / sqrt(pow(rotate_R,2)+1.0);
	rotate_dx = rotate_R * rotate_dy;
	
	pt_weapon[0].x = u->wep.x-rotate_dx;
	pt_weapon[0].y = u->wep.y+rotate_dy;

	pt_weapon[1].x = u->wep.x+rotate_dx;
	pt_weapon[1].y = u->wep.y-rotate_dy;

	pt_weapon[2].x = u->x-rotate_dx;
	pt_weapon[2].y = u->y+rotate_dy;

	// following condition causes mask of PlgBlt to flip upside down 
	//  and prints the weapon image with flipped mask
	//   we adjust it with tilting the coordinates.
	if ( pt_weapon[2].x == pt_weapon[0].x ) 
		pt_weapon[2].x += 1.0; 

	switch( u->wep.weapon_type ){
	case 1:
		PlgBlt(backS_dc, pt_weapon,wep_dc[1],0,0,20,60,wepm_bmp[1],0,0);
		break;
	case 5:
		PlgBlt(backS_dc, pt_weapon,wep_dc[5],0,0,15,40,wepm_bmp[5],0,0);
		break;		
	case 2:
		PlgBlt(backS_dc, pt_weapon,wep_dc[2],0,0,20,130,wepm_bmp[2],0,0);
		break;
	case 4:
		PlgBlt(backS_dc, pt_weapon,wep_dc[4],0,0,20,60,wepm_bmp[4],0,0);
		break;
	case 3:
		PlgBlt(backS_dc, pt_weapon,wep_dc[3],0,0,102,36,wepm_bmp[3],0,0);

		//draws the bow string
		if ( u->arwp != NULL ) {
			SelectObject(backS_dc, smallbluePen);

			pntArray[0].x = u->arwp->back.x;
			pntArray[0].y = u->arwp->back.y;
			pntArray[1].x = pt_weapon[2].x +( u->wep.x > u->x ? 
				rotate_dy*8.0/u->wep.WD->hwidth : -1*rotate_dy*8.0/u->wep.WD->hwidth);
			pntArray[1].y = pt_weapon[2].y +( u->wep.x > u->x ? 
				rotate_dx*8.0/u->wep.WD->hwidth : -1*rotate_dx*8.0/u->wep.WD->hwidth);//

			Polyline(backS_dc, pntArray, 2);

			pntArray[1].x = pt_weapon[2].x + 2.0*rotate_dx +( u->wep.x > u->x ? 
				rotate_dy*8.0/u->wep.WD->hwidth : -1*rotate_dy*8.0/u->wep.WD->hwidth);
			pntArray[1].y = pt_weapon[2].y - 2.0*rotate_dy +( u->wep.x > u->x ? 
				rotate_dx*8.0/u->wep.WD->hwidth : -1*rotate_dx*8.0/u->wep.WD->hwidth);//

			Polyline(backS_dc, pntArray, 2);
		}
			
		break;
	}

	//restore u->x and u->y
	u->x = temp_ux;
	u->y = temp_uy;
}

////////////////////
//Sparkling effects
/////////////////////
inline void Display::DrawWepSparkles(Unit *u){//, POINT pt_acur){	

#ifdef smode

	if ( u->userunit && u->dead ) 
		return;

#endif


	////////////////////////////////
	//Effects at the weapon tips
	///////////////////////////////
	SelectObject(backS_dc, nullPen);

	u->wep.skp_counter++;
	u->wep.skp_counter = u->wep.skp_counter % MAX_WEP_SPARKLE;
	u->wep.skp[u->wep.skp_counter].x =  u->wep.x + DISP_ADJ_X;
	u->wep.skp[u->wep.skp_counter].y =  u->wep.y + DISP_ADJ_Y;

	Sparkle_counter = &u->wep.skp_counter;
	/*
	if ( u->team == 1 )
		SparkleVar = Sparkle2;
	else 
		SparkleVar = Sparkle2;
		*/
	SparkleVar = Sparkle2;
	if ( u->dead ) 
		max_circle_size = 12.0;
	else if ( u->wep.Wep_status == Wep_on_dns ) {
		if ( u->wep.weapon_type == 1 ) 
			max_circle_size = u->wep.stance_charge/10;
		else if ( u->wep.weapon_type == 4 ) 
			max_circle_size = u->wep.stance_charge/20;
		else if ( u->wep.weapon_type == 5 ) 
			max_circle_size = u->wep.stance_charge/5;

		if ( u->team == 0 )
			SparkleVar = SparkleP;
		else 
			SparkleVar = SparkleG;
	}
	else if (  (u->wep.Wep_breaks_guard) 
			|| (u->wep.Wep_status == Wep_on_poke)  ) {
		max_circle_size = 7.0;
	}
	else
		max_circle_size = 2.5;

	for ( int i =0; i < MAX_WEP_SPARKLE; i ++ ) {
		(*Sparkle_counter)++;
		*Sparkle_counter = (*Sparkle_counter)% MAX_WEP_SPARKLE;
		SelectObject(backS_dc,  SparkleVar[i] );
		//u->wep.skp[j].x += rand()%2 ;// use it for moving particle ...
		//u->wep.skp[j].y += rand()%2 ;//
		circle_size = max_circle_size*(i)/MAX_WEP_SPARKLE;
		Ellipse(backS_dc, u->wep.skp[*Sparkle_counter].x - circle_size - DISP_ADJ_X, 
						  u->wep.skp[*Sparkle_counter].y - circle_size - DISP_ADJ_Y, 
						  u->wep.skp[*Sparkle_counter].x + circle_size - DISP_ADJ_X, 
						  u->wep.skp[*Sparkle_counter].y + circle_size - DISP_ADJ_Y);// RGB(50+i*10,0,0));
	}	
}

inline void Display::DrawWepHiteffect(){
	/////////////////////////////////
	//Draw HitEffects
	///////////////////////////////
	//SelectObject(backS_dc, blackPen);
	SelectObject(backS_dc, nullPen);
	for ( int i = 0; i < EFFECT_NUM; i++ ) {
		if ( disp_hef[i].timer > 0 ) {
			circle_size = (disp_hef[i].timer/2.0);
			if ( disp_hef[i].type == Hit_Normal ) 
				SelectObject(backS_dc,  red_brush );
			else if ( disp_hef[i].type == Hit_Guarded ) {
				SelectObject(backS_dc,  green2_brush );
				//circle_size *=2.0;
			}
			else if ( disp_hef[i].type == Hit_GuardBreak ) 
				SelectObject(backS_dc,  redorange_brush );
			else if ( disp_hef[i].type == Hit_DnSB ) {
					SelectObject(backS_dc,SparkleP[MAX_WEP_SPARKLE-1]);
					circle_size *= 2.0;
			}
			else if ( disp_hef[i].type == Hit_DnSG ) {
					SelectObject(backS_dc,SparkleG[MAX_WEP_SPARKLE-1]);
					circle_size *= 2.0;
			}
			
			Ellipse(backS_dc, disp_hef[i].pos.x-circle_size - DISP_ADJ_X, 
							  disp_hef[i].pos.y-circle_size - DISP_ADJ_Y, 
							  disp_hef[i].pos.x+circle_size - DISP_ADJ_X, 
							  disp_hef[i].pos.y+circle_size - DISP_ADJ_Y 
							  );//  u->wep.skp[j].y +circle_size);// RGB(50+i*10,0,0));
			disp_hef[i].timer--;
		}
	}
}

///////////////////////
// Draw_cursor
///////////////////////
void Display::DrawCursor( POINT pt_acur, int mode) {

	SelectObject(back_dc, nullPen);
	
	switch(mode){
		case 0://mouse cursor
			NUM_SPARKLE = MAX_CURSOR_SPARKLE;
			SparkleVar = Sparkle2;
			ceffect_x = cursoreffect_x;
			ceffect_y = cursoreffect_y;
			Sparkle_counter = &cursoreffect_counter;
			max_circle_size = 10.0;
			break;
		case 1://Red sparkles around LIVE button
			NUM_SPARKLE = MAX_WEP_SPARKLE;
			SparkleVar = SparkleP;
			ceffect_x = buttoneffect1_x;
			ceffect_y = buttoneffect1_y;
			Sparkle_counter = &buttoneffect1_counter;
			max_circle_size = 8.0;
			break;
		case 2://Blue sparkles around LIVE button
			NUM_SPARKLE = MAX_WEP_SPARKLE;
			SparkleVar = SparkleG;
			ceffect_x = buttoneffect2_x;
			ceffect_y = buttoneffect2_y;
			Sparkle_counter = &buttoneffect2_counter;
			max_circle_size = 8.0;
			break;
	}

	(*Sparkle_counter)++;
	(*Sparkle_counter) = (*Sparkle_counter) % NUM_SPARKLE;
	ceffect_x[(*Sparkle_counter)] = pt_acur.x; 
	ceffect_y[(*Sparkle_counter)] =  pt_acur.y;

	for ( int i =0; i < NUM_SPARKLE; i ++ ) {
		(*Sparkle_counter)++;
		*Sparkle_counter = (*Sparkle_counter)% NUM_SPARKLE;
		SelectObject(back_dc,  SparkleVar[i] );
		circle_size = max_circle_size*(i)/NUM_SPARKLE;
		Ellipse(back_dc,	ceffect_x[*Sparkle_counter] - circle_size,
							ceffect_y[*Sparkle_counter] - circle_size, 
							ceffect_x[*Sparkle_counter] + circle_size,
							ceffect_y[*Sparkle_counter] + circle_size);
	}
}

/////////////////////////
// Draw_weapon_reach
////////////////////////
void Display::DrawWepReach(Unit *u, POINT pt_acur, bool LButtonDown, bool RButtonDown){
	if ( *zoom < 100 ) //can be fixed for other cases where the action is for large AOE.
		return;

	//pt_acur.x = pt_cur.x - mpt.x + A->x -WWAD;
	//pt_acur.y = pt_cur.y - mpt.y + A->y -WHAD;

	pt_acur.x -= WWAD;
	pt_acur.y -= WHAD;

	wepr.x = u->x + pt_acur.x ;
	wepr.y = u->y + pt_acur.y  ;

	u->x -= DISP_ADJ_X;
	u->y -= DISP_ADJ_Y;
	wepr.x -= DISP_ADJ_X;
	wepr.y -= DISP_ADJ_Y;
	u->wep.x -= DISP_ADJ_X;
	u->wep.y -= DISP_ADJ_Y;

	if ( LButtonDown  ) {
		SelectObject(backS_dc, green3Pen);
		if (    sqrt( pow(  wepr.x - (u->x + (u->wep.y - u->y)), 2.0 ) + 
					  pow(  wepr.y - (u->y - (u->wep.x - u->x)), 2.0 ) )
				<
				sqrt( pow(  wepr.x - (u->x - (u->wep.y - u->y)), 2.0 ) + 
					  pow(  wepr.y - (u->y + (u->wep.x - u->x)), 2.0 ) )
				
			) {
			Arc(backS_dc, u->x - u->wep.WD->length, u->y - u->wep.WD->length,
						 u->x + u->wep.WD->length, u->y + u->wep.WD->length,
						 u->wep.x 				 , u->wep.y		 , 
						 //u->x + pt_acur.y		 , u->y - pt_acur.x );
						  wepr.x		     , wepr.y );
		}
		else {
			Arc(backS_dc, u->x - u->wep.WD->length, u->y - u->wep.WD->length,
						 u->x + u->wep.WD->length, u->y + u->wep.WD->length, 
						// u->x - pt_acur.y		 , u->y + pt_acur.x,
						  wepr.x		     ,  wepr.y,
						 u->wep.x 				 , u->wep.y		);
		}
	}
	if ( RButtonDown  ) {
		SelectObject(backS_dc, orangePen);
		if ( 
			sqrt( pow(  u->x + pt_acur.x - (u->x + (u->wep.y - u->y)), 2.0 ) + 
				  pow(  u->y + pt_acur.y - (u->y - (u->wep.x - u->x)), 2.0 ) )
			>
			sqrt( pow(  u->x + pt_acur.x - (u->x - (u->wep.y - u->y)), 2.0 ) + 
				  pow(  u->y + pt_acur.y - (u->y + (u->wep.x - u->x)), 2.0 ) )
			) {
	
			Arc(backS_dc, u->x - u->wep.WD->length, u->y - u->wep.WD->length,
						 u->x + u->wep.WD->length, u->y + u->wep.WD->length,
						 u->wep.x 				 , u->wep.y		 , 
						 u->x - pt_acur.y		 , u->y + pt_acur.x );
		}
		else {
			Arc(backS_dc, u->x - u->wep.WD->length, u->y - u->wep.WD->length,
						 u->x + u->wep.WD->length, u->y + u->wep.WD->length, 
						 u->x + pt_acur.y		 , u->y - pt_acur.x,
						 u->wep.x 				 , u->wep.y		);

		}
	
	}
	u->x += DISP_ADJ_X;
	u->y += DISP_ADJ_Y;
	wepr.x += DISP_ADJ_X;
	wepr.y += DISP_ADJ_Y;
	u->wep.x += DISP_ADJ_X;
	u->wep.y += DISP_ADJ_Y;
}


/////////////////////////
// Draw_arrow
////////////////////////
inline void Display::DrawArrow( ){
	
	//variables for rotated weapon sprite
	double rotate_R ;
	double rotate_dy ;
	double rotate_dx ;

	POINT pt_arrow [3];

	SelectObject(backS_dc, blackPen);

	for ( int i =0; i < MAXARROW; i++ ) {

		if ( disp_arw[i].active ) {		
			if ( disp_arw[i].front.x == disp_arw[i].back.x ) {
				rotate_dx = disp_arw[i].hwidth;
				rotate_dy = 0.0;
			}
			else if (disp_arw[i].front.y == disp_arw[i].back.y){
				rotate_dx = 0.0;
				rotate_dy = disp_arw[i].hwidth;
			}
			else {
				rotate_R  = (disp_arw[i].front.y - disp_arw[i].back.y) / (disp_arw[i].front.x - disp_arw[i].back.x);
				rotate_dy = disp_arw[i].hwidth / sqrt(pow(rotate_R,2)+1.0);
				rotate_dx = rotate_R * rotate_dy;
			}

			pt_arrow[0].x = disp_arw[i].front.x-rotate_dx ;// 
			pt_arrow[0].y = disp_arw[i].front.y+rotate_dy ;// 

			pt_arrow[1].x = disp_arw[i].front.x+rotate_dx ;//
			pt_arrow[1].y = disp_arw[i].front.y-rotate_dy ;// 

			pt_arrow[2].x = disp_arw[i].back.x-rotate_dx ;// 
			pt_arrow[2].y = disp_arw[i].back.y+rotate_dy ;// 
	
			PlgBlt(backS_dc, pt_arrow ,arrow_dc,0,0,10, 43,arrow_mask_bmp,0,0);
		}
	}



}

////////////////
// Title Logo
////////////////
void Display::DrawTitleLogo(POINT pt_acur){
	FillRect(back_dc, &back_rect, black_brush);
	BitBlt(back_dc,  85, 80, 600, 300, titlelogo_dc, 
				0, 0, SRCCOPY);

	if (   pt_acur.x > WWAD + 100
		&& pt_acur.x < WWAD + 250 
		&& pt_acur.y > WHAD
		&& pt_acur.y < WHAD + 50 ) {

		Rectangle(back_dc,WWAD + 100, WHAD + 30,
			              WWAD + 230, WHAD + 40);
	}
	else if (   pt_acur.x > WWAD + 100
		&& pt_acur.x < WWAD + 250 
		&& pt_acur.y > WHAD - 60
		&& pt_acur.y < WHAD - 10 ) {

		Rectangle(back_dc,WWAD + 120, WHAD - 30 ,
			              WWAD + 250, WHAD - 20);
	}

	//References
	SetTextColor(back_dc, RGB(255,255,255));  // white text color
	SetBkColor(back_dc, RGB(0,0,0)); // black background
	_stprintf_s(Disp_text, _T("Image Sprites are from"));
	TextOut(back_dc, 23 + WWAD-200,  WHAD + 160, Disp_text, _tcslen(Disp_text));

	_stprintf_s(Disp_text, _T("http://opengameart.org/content/one-more-lpc-alternate-character"));
	TextOut(back_dc, 23 + WWAD-150,  WHAD + 180, Disp_text, _tcslen(Disp_text));

	_stprintf_s(Disp_text, _T("http://opengameart.org/content/alternate-lpc-character-sprites-george"));
	TextOut(back_dc, 23 + WWAD-150,  WHAD + 200, Disp_text, _tcslen(Disp_text));

	_stprintf_s(Disp_text, _T("http://opengameart.org/content/420-pixel-art-icons-for-medievalfantasy-rpg"));
	TextOut(back_dc, 23 + WWAD-150,  WHAD + 220, Disp_text, _tcslen(Disp_text));
}

///////////////////////////
// Draw Background
//////////////////////////
void Display::DrawBackground(){

	SelectObject(backS_dc, white_brush);
	SelectObject(backS_dc, bluePen);

#ifdef debug_mode
	if ( *zoom >= 100 ) {
		SelectObject(backS_dc, blue2Pen);
		DrawBGGridLine(100);
	}
#endif

	if ( PRINT_ICON ){
		
		if ( *zoom >= 40 ) {
			SelectObject(back_dc, blue3iPen);
			DrawBGGridLineIcon(400);
		}
		if ( *zoom >= 10 ) {
			SelectObject(back_dc, blue4iPen);
			DrawBGGridLineIcon(800);
		}

		if ( *zoom >= 10 ) 
			SelectObject(back_dc, blue5iPen);
		else 
			SelectObject(back_dc, blue5iiPen);

		DrawBGGridLineIcon(3200);
	}
	else {
		if ( *zoom >= 50 ) {
			SelectObject(backS_dc, blue3Pen);
			DrawBGGridLine(400);
		}
		if ( *zoom >= 10 ) {
			SelectObject(backS_dc, blue4Pen);
			DrawBGGridLine(800);
		}

		SelectObject(backS_dc, blue5Pen);
		DrawBGGridLine(3200);
	}
}

//draws the grid line
inline void Display::DrawBGGridLine(int grid_size) {

	pntArray[0].y = 0;	
	pntArray[1].y = WINDOW_HEIGHT;
	for ( int i = disp_center->x / grid_size - WINDOW_WIDTH/grid_size-2; 
				i < disp_center->x / grid_size + WINDOW_WIDTH/grid_size+2; i++ ) {
		
		pntArray[0].x = grid_size*i-DISP_ADJ_X;
		if ( pntArray[0].x < -100 || pntArray[0].x > WINDOW_WIDTH +100 )
			continue;
		pntArray[1].x = pntArray[0].x;			
		Polyline(backS_dc, pntArray, 2);	
	}

	pntArray[0].x = 0;			
	pntArray[1].x = WINDOW_WIDTH;		
	for ( int i = disp_center->y / grid_size - WINDOW_HEIGHT/grid_size-2; 
				i < disp_center->y / grid_size + WINDOW_HEIGHT/grid_size+2; i++ ) {
				
		pntArray[0].y =  grid_size*i-DISP_ADJ_Y;	
		if ( pntArray[0].y < -100 || pntArray[0].x > WINDOW_HEIGHT + 100)
			continue;		
		pntArray[1].y =  pntArray[0].y;
		Polyline(backS_dc, pntArray, 2);	
	}
}

//draws the grid line for icon mode
inline void Display::DrawBGGridLineIcon(int grid_size) {

	pntArray[0].y = 0;
	pntArray[1].y = WINDOW_HEIGHT;
	for ( int i = (disp_center->x / grid_size) - WINDOW_WIDTH*zoomed/grid_size; //can be compiler dependent.....
		      i <= (disp_center->x / grid_size) + WINDOW_WIDTH*zoomed/grid_size; i++ ) {
		pntArray[0].x = WWAD - (disp_center->x - grid_size*i)/zoomed;
		if ( pntArray[0].x < 0 || pntArray[0].x > WINDOW_WIDTH )
			continue;
		pntArray[1].x = pntArray[0].x;									

		Polyline(back_dc, pntArray, 2);	
	}

	pntArray[0].x = 0;
	pntArray[1].x = WINDOW_WIDTH;
	for ( int i = (disp_center->y / grid_size) - WINDOW_HEIGHT*zoomed/grid_size; 
			  i <= (disp_center->y / grid_size) + WINDOW_HEIGHT*zoomed/grid_size; i++ ) {

		pntArray[0].y =  WHAD - (disp_center->y - grid_size*i)/zoomed;
		if ( pntArray[0].y <0 || pntArray[0].x > WINDOW_HEIGHT )
			continue;
		pntArray[1].y =  pntArray[0].y;
		Polyline(back_dc, pntArray, 2);	
	}
}

////////////////////
//  calculates which units to be drawn on the screen 
////////////////////
void Display::DrawUnitsToDraw(LGroup* L){
	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {

		if ( !L[k].active || !L[k].within_battle_range )
			continue;

		for (int i =0; i <= MAX_NUM_UNIT; i++ ) {
			//if ( L[k].u[i].active && dist( (*disp_center), L[k].u[i] )*(*zoom)/100 < (WWAD*1.5) ) 
			if ( L[k].u[i].active 
				&& abs(disp_center->x - L[k].u[i].x)*(*zoom)/100 < (WWAD*1.3)
				&& abs(disp_center->y - L[k].u[i].y)*(*zoom)/100 < (WHAD*1.3)) 
				L[k].u[i].on_screen = true;
			else 
				L[k].u[i].on_screen = false;
		}		
	}
}

/////////////////////
// Draws Field  (stretched)
/////////////////////
void Display::DrawField(LGroup *L){

	SelectObject(back_dc, yellow_dot_Pen);
	SetBkColor(backS_dc, RGB(0,0,0)); // black background
	SetTextColor(backS_dc, RGB(255,255,255));  // black text color
	SetBkColor(back_dc, RGB(0,0,0)); // black background
	SetTextColor(back_dc, RGB(255,255,255));  // black text color

	Unit *u;

	//at min-zoom
	if (PRINT_LICON ){
		for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
			if ( !L[k].active  )
				continue;

			DrawUnit_LICON(&L[k]);
		}
		return;
	}

	DrawUnitsToDraw(L);

	//at mid-zoom
	if (PRINT_ICON) {
		for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
			if ( !L[k].active || !L[k].within_battle_range )
				continue;

			u = L[k].u;
			DrawUnit_ICON(u);
		}
		return;
	}

	//at max-zoom

	///////////////
	//Adjustments
	///////////////

	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].active || !L[k].within_battle_range )
			continue;

		u = L[k].u;
	
		for ( int i = 0; i <= MAX_NUM_UNIT; i++ ) {
			if ( !u[i].on_screen )
				continue;
			//adjustments to the unit positions for the display.
			u[i].x -= DISP_ADJ_X;
			u[i].y -= DISP_ADJ_Y;

			u[i].wep.x -= DISP_ADJ_X;
			u[i].wep.y -= DISP_ADJ_Y;

			u[i].wep.pulled_p.x -= DISP_ADJ_X;
			u[i].wep.pulled_p.y -= DISP_ADJ_Y;

			//unit position adjustment for better weapon position
			if ( NON_PULLED_WEP_UA ) {
				//restored later
				if ( u[i].team ==  0 )
					u[i].x -= unit_position_adj_x;
				else 
					u[i].x += unit_position_adj_x;
				u[i].y -= unit_position_adj_y;
			}

		}
	}
	
	for ( int i =0; i < MAXARROW; i++ ) {
		if ( disp_arw[i].active ) {
			disp_arw[i].front.x -= DISP_ADJ_X;
			disp_arw[i].front.y -= DISP_ADJ_Y;
			disp_arw[i].back.x  -= DISP_ADJ_X;
			disp_arw[i].back.y  -= DISP_ADJ_Y;
		}
	}

	/////////////////
	//Prints Units / Weapon / Effects / Arrows
	/////////////////
	
	


	//Units
	//  divided into 3 parts with the hope it will run more efficiently. 
	//  .... it didn't show better performance :( 
	//   we can try to have different loops for the teams, 
	//   but it rather seems that the weap is causing more problems.

	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].active || !L[k].within_battle_range )
			continue;
		u = L[k].u;

		for (int i =0; i <= MAX_NUM_UNIT; i++ ) {
			if ( L[k].u[i].on_screen ) {//if the unit is far away, we don't draw her
			
				if ( L[k].u[i].userunit && L[k].u[i].dead ){}
				else
					DrawUnit_Mask(&L[k].u[i]);
			}
		}
	}

	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].active || !L[k].within_battle_range )
			continue;
		u = L[k].u;

		for (int i =0; i <= MAX_NUM_UNIT; i++ ) {
			if ( DRAW_U )//if the unit is far away, we don't draw her
				DrawUnit_Unit(&L[k].u[i]);
		}
	}

	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].active || !L[k].within_battle_range )
			continue;
		u = L[k].u;

		for (int i =0; i <= MAX_NUM_UNIT; i++ ) {
			if ( DRAW_U )//if the unit is far away, we don't draw her
				DrawUnit_Hat(&L[k].u[i]);
		}
	}

	//Weapons
	DrawArrow();
	SelectObject(backS_dc, nullPen);

	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].active || !L[k].within_battle_range )
			continue;

		u = L[k].u;

		for ( int i =0; i <= MAX_NUM_UNIT; i++ ) {
			if ( DRAW_U )
				DrawWeapon(&L[k].u[i]);
		}
	}

	//Effects

	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].active || !L[k].within_battle_range )
			continue;

		for ( int i =0; i <= MAX_NUM_UNIT; i++ ) {
			if ( (L[k].u[i].on_screen && L[k].u[i].wep.Wep_status == Wep_on_dns) ) {
				DrawWepSparkles(&L[k].u[i]);
			}
		}
	}

	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].active || !L[k].within_battle_range )
			continue;
		u = L[k].u;

		for (int i =0; i <= MAX_NUM_UNIT; i++ ) {
			if ( L[k].u[i].on_screen )//&& !L[k].u[i].dead )//if the unit is far away, we don't draw her
				DrawUnit_Peri(&L[k].u[i]);
		}
	}	

	DrawWepHiteffect();
	
	////////////////////////
	//Reverses the adjustments
	//////////////////////////////
	
	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
		if ( !L[k].active || !L[k].within_battle_range )
			continue;

		u = L[k].u;

		for ( int i = 0; i <= MAX_NUM_UNIT; i++ ) {
			if ( !u[i].on_screen )
				continue;
			u[i].x += DISP_ADJ_X;
			u[i].y += DISP_ADJ_Y;

			u[i].wep.x += DISP_ADJ_X;
			u[i].wep.y += DISP_ADJ_Y;

			u[i].wep.pulled_p.x += DISP_ADJ_X;
			u[i].wep.pulled_p.y += DISP_ADJ_Y;	

			if ( NON_PULLED_WEP_UA ) {
				//restoring the modified value
				if ( u[i].team ==  0 )
					u[i].x += unit_position_adj_x;
				else 
					u[i].x -= unit_position_adj_x;
				u[i].y += unit_position_adj_y;
			}
		}			
	}

	for ( int i =0; i < MAXARROW; i++ ) {
		if ( disp_arw[i].active ) {
			disp_arw[i].front.x += DISP_ADJ_X;
			disp_arw[i].front.y += DISP_ADJ_Y;
			disp_arw[i].back.x  += DISP_ADJ_X;
			disp_arw[i].back.y  += DISP_ADJ_Y;
		}
	}

	//StretchBlt from backS_dc to back_dc
	StretchBlt(
		back_dc, 
		0,0,
		WINDOW_WIDTH ,WINDOW_HEIGHT, 

		backS_dc, 
		WWAD- WWAD*100/ (*zoom)  ,  WHAD - WHAD*100/ (*zoom),
		WINDOW_WIDTH*100/(*zoom) ,WINDOW_HEIGHT*100/(*zoom) , 

		SRCCOPY);

	//Square around user unit
	double sq_size = 20.0 / (100.0/(*zoom));

	for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {	
		if ( !L[k].u[MAX_NUM_UNIT].userunit )
			continue;	

		if ( !L[k].u[MAX_NUM_UNIT].dead ) {

			if ( L[k].u[MAX_NUM_UNIT].team == 0 ) {
				dummy.x = WWAD ;
				SelectObject(back_dc, yellow_dot_Pen);
			}
			else {
				dummy.x = WWAD;
				SelectObject(back_dc, blue_dot_Pen);
			}
			dummy.y = WHAD ;

			if (L[k].u[MAX_NUM_UNIT].wep.weapon_type != 3){
				if ( L[k].u[MAX_NUM_UNIT].team == 0 )
					dummy.x -= unit_position_adj_x / (100.0/(*zoom));
				else 
					dummy.x += unit_position_adj_x / (100.0/(*zoom));
				dummy.y -= unit_position_adj_y / (100.0/(*zoom));
			}

			pntArray[0].x = dummy.x - sq_size;			pntArray[0].y = dummy.y - sq_size;	
			pntArray[1].x = dummy.x - sq_size;			pntArray[1].y = dummy.y + sq_size;
			Polyline(back_dc, pntArray, 2);		
			pntArray[1].x = dummy.x + sq_size;			pntArray[1].y = dummy.y - sq_size;
			Polyline(back_dc, pntArray, 2);	
			pntArray[0].x = dummy.x + sq_size;			pntArray[0].y = dummy.y + sq_size;	
			Polyline(back_dc, pntArray, 2);	
			pntArray[1].x = dummy.x - sq_size;			pntArray[1].y = dummy.y + sq_size;
			Polyline(back_dc, pntArray, 2);	

		}
		break;	
	}
}


////////////////////////////
// Draws LGroups for zoomed-out battlefield
///////////////////////////
inline void Display::DrawUnit_LICON(LGroup *L){

	if (L->empty)
		return;

	pntArray[0].x = DRAW_ICON_COOR_X(L->center.x);//disp_center->x;
	pntArray[0].y = DRAW_ICON_COOR_Y(L->center.y);//disp_center->y;
	
	dummy_dx = 30;
	dummy_dy = 30;		

	if ( L->team == 0) {

		SelectObject(back_dc, yellow_solid_Pen);	
		pntArray[1].x = pntArray[0].x - dummy_dx; 
		pntArray[1].y = pntArray[0].y - dummy_dy;
		Polyline(back_dc,pntArray,2);

		BitBlt(back_dc, pntArray[0].x-24 -dummy_dx, pntArray[0].y-24 -dummy_dy, 48, 48, betty_m_dc, 
				0, 0, SRCPAINT);

		BitBlt(back_dc, pntArray[0].x-24 -dummy_dx, pntArray[0].y-24 -dummy_dy, 48, 48, betty_dc, 
				0, 0, SRCAND);
	}
	else{

		SelectObject(back_dc, blue_solid_Pen);
		pntArray[1].x = pntArray[0].x + dummy_dx; 
		pntArray[1].y = pntArray[0].y + dummy_dy;
		Polyline(back_dc,pntArray,2);

		BitBlt(back_dc, pntArray[0].x-24 +dummy_dx, pntArray[0].y-24 +dummy_dy, 48, 48, george_m_dc, 
				0, 0, SRCPAINT);

		BitBlt(back_dc, pntArray[0].x-24 +dummy_dx, pntArray[0].y-24 +dummy_dy, 48, 48, george_dc, 
				0, 0, SRCAND);
	}
	
	//Rectangle around Unit Icon
	//SetBkColor(back_dc, RGB(0,0,0)); // black background
	dummy_dx = 4;
	dummy_dy = 4;
	if ( L->within_battle_range && L->print_e_num() > 0 ) {
		if ( L->team == 0 ) 
			SelectObject(back_dc, yellow_dot_Pen);
		else 
			SelectObject(back_dc, blue_dot_Pen);
		pntArray[4].x = DRAW_ICON_COOR_X(L->boundary[0].x);
		pntArray[4].y = DRAW_ICON_COOR_Y(L->boundary[0].y);
		pntArray[2].x = DRAW_ICON_COOR_X(L->boundary[3].x);
		pntArray[2].y = DRAW_ICON_COOR_Y(L->boundary[3].y);
		pntArray[3].x = DRAW_ICON_COOR_X(L->boundary[2].x);
		pntArray[3].y = DRAW_ICON_COOR_Y(L->boundary[2].y);
		pntArray[1].x = DRAW_ICON_COOR_X(L->boundary[1].x);
		pntArray[1].y = DRAW_ICON_COOR_Y(L->boundary[1].y);
		pntArray[0].x = DRAW_ICON_COOR_X(L->boundary[0].x);
		pntArray[0].y = DRAW_ICON_COOR_Y(L->boundary[0].y);
	}
	else {
		pntArray[4].x = pntArray[0].x - dummy_dx; pntArray[4].y = pntArray[0].y - dummy_dy;
		pntArray[3].x = pntArray[0].x - dummy_dx; pntArray[3].y = pntArray[0].y + dummy_dy;
		pntArray[2].x = pntArray[0].x + dummy_dx; pntArray[2].y = pntArray[0].y + dummy_dy;
		pntArray[1].x = pntArray[0].x + dummy_dx; pntArray[1].y = pntArray[0].y - dummy_dy;
		pntArray[0].x = pntArray[0].x - dummy_dx; pntArray[0].y = pntArray[0].y - dummy_dy;
	}
	Polyline(back_dc,pntArray,5);

	//#ifdef debug_mode
		//group[0] Id on the icons
		L->calc_nmember();
		_stprintf_s(Disp_text, _T("%3.0lf"), L->num_member);	
		DrawText(Disp_text,pntArray[0].x-23, pntArray[0].y+3, back_dc);

		//_stprintf_s(Disp_text, _T("%d"), L->team);	
		//DrawText(Disp_text,pntArray[0].x-23, pntArray[0].y+23, back_dc);

		if ( L->team == 0 ) 
			SelectObject(back_dc, yellow_dot_Pen);
		else 
			SelectObject(back_dc, blue_dot_Pen);
		pntArray[0].x = DRAW_ICON_COOR_X(L->center.x) - L->team*3;					
		pntArray[0].y = DRAW_ICON_COOR_Y(L->center.y) - L->team*3;	
		pntArray[1].x = DRAW_ICON_COOR_X(L->uDefaultDestination.x) - L->team*3;
		pntArray[1].y = DRAW_ICON_COOR_Y(L->uDefaultDestination.y) - L->team*3;
		Polyline(back_dc, pntArray, 2);	

	//#endif
}

////////////////////////////
// Draw Unit ICON : draws icon of the unit for zoomed-out battlefield
///////////////////////////
inline void Display::DrawUnit_ICON(Unit * u){

	//Prints Unit Icons only
	for ( int i = 0 ; i <= MAX_NUM_UNIT; i++ ) {
		if ( !DRAW_U )
			continue;

		pntArray[0].x = DRAW_ICON_COOR_X(u[i].x);//disp_center->x;
		pntArray[0].y = DRAW_ICON_COOR_Y(u[i].y);//disp_center->y;

		BitBlt(back_dc, 
		pntArray[0].x-17, 
		pntArray[0].y-17, 
		34, 34, wep_mini_dc[u[i].wep.weapon_type], 
			0, 0, SRCCOPY);

		//Rectangle around Unit Icon
		if ( u[i].team == 0 ) {
			if ( u[i].userunit ) 
				SelectObject(back_dc, yellow_dot_Pen);
			else 
				SelectObject(back_dc, yellow_solid_Pen);
		}
		else {
			if ( u[i].userunit ) 
				SelectObject(back_dc, blue_dot_Pen);
			else 
				SelectObject(back_dc, blue_solid_Pen);
		}
		
		SetBkColor(back_dc, RGB(0,0,0)); // black background
		dummy_dx = 17;
		dummy_dy = 17;
		pntArray[4].x = pntArray[0].x - dummy_dx; pntArray[4].y = pntArray[0].y - dummy_dy;
		pntArray[3].x = pntArray[0].x - dummy_dx; pntArray[3].y = pntArray[0].y + dummy_dy;
		pntArray[2].x = pntArray[0].x + dummy_dx; pntArray[2].y = pntArray[0].y + dummy_dy;
		pntArray[1].x = pntArray[0].x + dummy_dx; pntArray[1].y = pntArray[0].y - dummy_dy;
		pntArray[0].x = pntArray[0].x - dummy_dx; pntArray[0].y = pntArray[0].y - dummy_dy;

		Polyline(back_dc,pntArray,5);

		#ifdef debug_mode
			_stprintf_s(Disp_text, _T("e:%d"), Maru_L[u[i].group[1]].print_e_num());	
			DrawText(Disp_text,pntArray[0].x-23, pntArray[0].y+23, back_dc);

			_stprintf_s(Disp_text, _T("f:%d"), Maru_L[u[i].group[1]].print_f_num());	
			DrawText(Disp_text,pntArray[0].x-23, pntArray[0].y+3, back_dc);
		
			//group[0] Id on the icons
			_stprintf_s(Disp_text, _T("%d"), Maru_L[u[i].group[1]].g[u[i].group[0]].print_num_member());	
			DrawText(Disp_text,pntArray[0].x+23, pntArray[0].y+3, back_dc);

			_stprintf_s(Disp_text, _T("%d-%d"), u[i].group[1], u[i].group[0]);	
			DrawText(Disp_text,pntArray[0].x+3, pntArray[0].y+23, back_dc);

			_stprintf_s(Disp_text, _T("%lf"), Maru_L[u[i].group[1]].num_member);	
			//DrawText(Disp_text,pntArray[0].x-23, pntArray[0].y+43, back_dc);
		#endif
			if ( !u[i].userunit ){//.Target->active ) {
				if ( u[i].team == 0 ) 
					SelectObject(back_dc, yellow_dot_Pen);
				else 
					SelectObject(back_dc, blue_dot_Pen);
				pntArray[0].x = DRAW_ICON_COOR_X(u[i].x) - u[i].team*3;					
				pntArray[0].y = DRAW_ICON_COOR_Y(u[i].y) - u[i].team*3;	
				pntArray[1].x = DRAW_ICON_COOR_X(u[i].Target->x) - u[i].team*3;
				pntArray[1].y = DRAW_ICON_COOR_Y(u[i].Target->y) - u[i].team*3;
				Polyline(back_dc, pntArray, 2);	
			}
		
	}
}

////////////////////////////
// Draw Unit - draws masks
///////////////////////////
inline void Display::DrawUnit_Mask(Unit *u){

	if ( u->team == 0) {
		BitBlt(backS_dc, u->x-24 , u->y-24, 48, 48, betty_m_dc, 
				u->direction*48, u->sequence*48, SRCPAINT);
	}
	else{
		BitBlt(backS_dc, u->x-24, u->y-24, 48, 48, george_m_dc, 
				u->direction*48, u->sequence*48, SRCPAINT);
	}
}

////////////////////////////
// Draw Unit - draws the actual units
///////////////////////////
inline void Display::DrawUnit_Unit(Unit *u){
	if ( u->team == 0) {
		BitBlt(backS_dc, u->x-24, u->y-24, 48, 48, betty_dc, 
				u->direction*48, u->sequence*48, SRCAND);
	}
	else{
		BitBlt(backS_dc, u->x-24, u->y-24, 48, 48, george_dc, 
				u->direction*48, u->sequence*48, SRCAND);
	}
}

////////////////////////////
// Draw Hat - draws hats...
///////////////////////////
inline void Display::DrawUnit_Hat(Unit *u){

	int hat_type;

	switch(u->wep.weapon_type){
	case 4:
		hat_type = 2;
		break;
	case 5:
		hat_type = 0;
		break;
	default:
		hat_type = 1;
		break;
	}

	BitBlt(backS_dc, u->x-17, u->y-26, 34, 34, hat_m_dc[hat_type], 
				u->direction*34, 0, SRCPAINT);

	BitBlt(backS_dc, u->x-17, u->y-26, 34, 34, hat_dc[hat_type], 
				u->direction*34, 0, SRCAND);
}

////////////////////////////
// Draw Unit - draws peripherals
///////////////////////////
inline void Display::DrawUnit_Peri(Unit *u){

	//////////////
	//Guard sign
	if ( (u->wep.Wep_status == Wep_on_guard) ) {
		BitBlt(backS_dc, u->x-32, u->y-20, 17, 17, shield_m_dc, 
				0, 0, SRCAND);
		BitBlt(backS_dc, u->x-32, u->y-20, 17, 17, shield_dc, 
				0, 0, SRCPAINT);
	}

	/////////////
	// Slow sign
	if ( u->status_slow > 0 ) {
		BitBlt(backS_dc, u->x-32, u->y-20+25, 17, 17, status_m_dc, 
				0, 0, SRCAND);
		BitBlt(backS_dc, u->x-32, u->y-20+25, 17, 17, status_dc, 
				0, 0, SRCPAINT);
		
		//slow bar
		SelectObject(backS_dc, yellow_solid_Pen);
		pntArray[0].x = u->x-32;							pntArray[0].y = u->y + 21;	
		pntArray[1].x = u->x-32 + 17*u->status_slow / 100.0;	pntArray[1].y = u->y + 21;
		Polyline(backS_dc, pntArray, 2);	
	}

#ifdef debug_mode
	//AI action type. for debugging purposes
	if ( *zoom >= 100 ) {
		_stprintf_s(Disp_text, _T("G: %d"), u->group[0]);
		DrawText(Disp_text,u->x-80, u->y + 0, backS_dc);
		_stprintf_s(Disp_text, _T("Pre : %d, %d"), u->AI_preset[0], u->AI_preset[1]);
		DrawText(Disp_text,u->x-20, u->y + 20, backS_dc);
		_stprintf_s(Disp_text, _T("Seq : %d"), u->AI_sequence);
		DrawText(Disp_text,u->x-20, u->y + 40, backS_dc);
		_stprintf_s(Disp_text, _T("ACT : %d"), u->ACT_type);
		DrawText(Disp_text,u->x-20, u->y + 60, backS_dc);
		_stprintf_s(Disp_text, _T("MOV : %d"), u->MV_type);
		DrawText(Disp_text,u->x-20, u->y + 80, backS_dc);
		_stprintf_s(Disp_text, _T("TAR : %d"), u->wep.stance);
		DrawText(Disp_text,u->x-20, u->y + 100, backS_dc);
	}

	if ( u->T_second != u && u->T_second->on_screen ) {
		SelectObject(backS_dc, blue2Pen);
		pntArray[0].x = u->x;					pntArray[0].y = u->y;	
		pntArray[1].x = u->T_second->x;			pntArray[1].y = u->T_second->y;
		Polyline(backS_dc, pntArray, 2);	
	}
	/*
	if ( u->Target->active ) {
		if ( u->team == 0 ) 
			SelectObject(backS_dc, yellow_dot_Pen);
		else 
			SelectObject(backS_dc, blue_dot_Pen);
		pntArray[0].x = u->x;					pntArray[0].y = u->y;	
		pntArray[1].x = u->Target->x;			pntArray[1].y = u->Target->y;
		Polyline(backS_dc, pntArray, 2);	
	}*/

	_stprintf_s(Disp_text, _T("TBD : %d"), u->TargetedBy);
	DrawText(Disp_text,u->x-20, u->y + 80);
	//_stprintf(Disp_text, _T("Target : %lf"), u->Target->x);
	//DrawText(Disp_text,u->x-20, u->y + 80);

	//For debugging
	SelectObject(backS_dc, nullPen);
	if ( !u->wep.Wep_breaks_guard  )//( u->T_second == NULL )//->wep.direction)//.has_swing_plan )//.stance != 0  ) 
		SelectObject(backS_dc, blue_brush);
	else {
		if ( u->stamina < 30 )
			SelectObject(backS_dc, red_brush);
		else
			SelectObject(backS_dc, yellow_brush);
	}
	Rectangle(backS_dc, u->x-70, u->y+30, u->x-50, u->y+40 );
#endif
	
	SelectObject(backS_dc, nullPen);

	///////////////////
	// Draws stance shift timer
	///////////////////
	if ( u->wep.stance_shift ) {

		SelectObject(backS_dc, white_brush); // background
		Rectangle(backS_dc,u->x - 10, u->y + 20, 
						   u->x - 10 + 25, u->y + 26);

		SelectObject(backS_dc, yellow_brush); // the stamina bar
		Rectangle(backS_dc,u->x - 10, u->y + 20, 
						   u->x - 10 + u->wep.stance_charge/4, u->y + 26);
	}

	//////////////
	//  HP bar
	//////////////	
	if ( u->team == 0 ) 
		SelectObject(backS_dc,SparkleP[MAX_WEP_SPARKLE-1]);
	else
		SelectObject(backS_dc,SparkleG[MAX_WEP_SPARKLE-1]);

	Rectangle(backS_dc, u->x - 17    , u->y - 14,     u->x - 17 + 5, u->y - 9  );

	//Current Max is 600
	if ( u->stat_currentHP > 500 ) {
		SelectObject(backS_dc, darkgreen_brush); 
		Rectangle(backS_dc, u->x - 17, u->y - DISP_HP_BAR_Y_BOT, u->x - 17+ 35 , u->y - DISP_HP_BAR_Y_TOP );	
		SelectObject(backS_dc, blue_brush); 
		Rectangle(backS_dc, u->x - 17, u->y - DISP_HP_BAR_Y_BOT, u->x - 17+ 35*(u->stat_currentHP-500)/100 , u->y - DISP_HP_BAR_Y_TOP );
	}
	else if ( u->stat_currentHP > 400 ) {
		SelectObject(backS_dc, lightgreen_brush); 
		Rectangle(backS_dc, u->x - 17, u->y - DISP_HP_BAR_Y_BOT, u->x - 17+ 35 , u->y - DISP_HP_BAR_Y_TOP );	
		SelectObject(backS_dc, darkgreen_brush); 
		Rectangle(backS_dc, u->x - 17, u->y - DISP_HP_BAR_Y_BOT, u->x - 17+ 35*(u->stat_currentHP-400)/100 , u->y - DISP_HP_BAR_Y_TOP );
	}
	else if ( u->stat_currentHP > 300 ) {
		SelectObject(backS_dc, yellow_brush); 
		Rectangle(backS_dc, u->x - 17, u->y - DISP_HP_BAR_Y_BOT, u->x - 17+ 35 , u->y - DISP_HP_BAR_Y_TOP );	
		SelectObject(backS_dc, lightgreen_brush); 
		Rectangle(backS_dc, u->x - 17, u->y - DISP_HP_BAR_Y_BOT, u->x - 17+ 35*(u->stat_currentHP-300)/100 , u->y - DISP_HP_BAR_Y_TOP );
	}
	else if ( u->stat_currentHP > 200 ){
		SelectObject(backS_dc, orange_brush); 
		Rectangle(backS_dc, u->x - 17, u->y - DISP_HP_BAR_Y_BOT, u->x - 17+ 35 , u->y - DISP_HP_BAR_Y_TOP );	
		SelectObject(backS_dc, yellow_brush); 
		Rectangle(backS_dc, u->x - 17, u->y - DISP_HP_BAR_Y_BOT, u->x - 17+ 35*(u->stat_currentHP-200)/100 , u->y - DISP_HP_BAR_Y_TOP );
	}
	else if ( u->stat_currentHP > 100 ){
		SelectObject(backS_dc, red_brush); 
		Rectangle(backS_dc, u->x - 17, u->y - DISP_HP_BAR_Y_BOT, u->x - 17+ 35 , u->y - DISP_HP_BAR_Y_TOP );	
		SelectObject(backS_dc, orange_brush); 
		Rectangle(backS_dc, u->x - 17, u->y - DISP_HP_BAR_Y_BOT, u->x - 17+ 35*(u->stat_currentHP-100)/100 , u->y - DISP_HP_BAR_Y_TOP );
	}
	else{
		SelectObject(backS_dc, red_brush); 
		Rectangle(backS_dc, u->x - 17, u->y - DISP_HP_BAR_Y_BOT, u->x - 17+ 35*u->stat_currentHP/100 , u->y - DISP_HP_BAR_Y_TOP );
	}

	if (false){//u->wep.weapon_type != 3) {
		// Draws weapon move effect
		pntArray[0].x=u->wep.x;
		pntArray[0].y=u->wep.y;
		pntArray[1].x=u->wep.old.x;
		pntArray[1].y=u->wep.old.y;

		if ( (u->wep.Wep_status == Wep_on_swing) ) 
			SelectObject(backS_dc, redPen);
		else 
			SelectObject(backS_dc, bluePen);

		Polyline(backS_dc, pntArray, 2);
	}
}

//////////////////////
// Draw_Button
//////////////////////
void Display::DrawButton(bool pause, int respawn_timer){
	SelectObject(back_dc, nullPen);
	SetTextColor(back_dc, RGB(0,0,0));  // black text color
	SetBkColor(back_dc, RGB(255,255,255)); // black background
	//buttons.

	if ( pause ) {
		BitBlt(back_dc, 10, 10, 80, 80, button_m_dc[1], 	0, 0, SRCPAINT);
		BitBlt(back_dc, 10, 10, 80, 80, button_dc[1], 	0, 0, SRCAND);
		/*
		SelectObject(back_dc, yellowB_brush);
		SetBkColor(back_dc, RGB(255,255,255)); // black background
		Ellipse(back_dc, 20 , 20 , 
					   80 , 80  );
		_stprintf_s(Disp_text, _T("PAUSED"));
		TextOut(back_dc, 23 , 40 , Disp_text, _tcslen(Disp_text));
		*/
	}
	else { 
		BitBlt(back_dc, 10, 10, 80, 80, button_m_dc[2], 	0, 0, SRCPAINT);
		BitBlt(back_dc, 10, 10, 80, 80, button_dc[2], 	0, 0, SRCAND);
		/*
		SelectObject(back_dc, redB_brush);
		SetBkColor(back_dc, RGB(120,120,120)); // black background

		Ellipse(back_dc, 20, 20, 
					   80, 80);
		_stprintf_s(Disp_text, _T("LIVE"));
		TextOut(back_dc, 35 , 40 , Disp_text, _tcslen(Disp_text));
		*/
	}

	
	//SetBkColor(back_dc, RGB(0,0,0)); // black background
	//SetTextColor(backS_dc, RGB(255,255,255));  // black text color

	BitBlt(back_dc, 10, 460, 80, 80, button_m_dc[0], 	0, 0, SRCPAINT);
	BitBlt(back_dc, 10, 460, 80, 80, button_dc[0], 	0, 0, SRCAND);
/*
	SelectObject(back_dc, greenB_brush);
	SetBkColor(back_dc, RGB(210,210,210)); // black background
	Ellipse(back_dc, 10, 460 , 
					   90, 540 );	
	_stprintf_s(Disp_text, _T("To Title"));
	TextOut(back_dc, 25 , 490 , Disp_text, _tcslen(Disp_text));
*/
	//sparkling effects around the buttons
	pt_buttoneffect.x = 32 * cos(respawn_timer / RESPAWN_PERIOD_D *6.0*mPI+mPI/2.0) +50 ;
	pt_buttoneffect.y = 32 * sin(respawn_timer / RESPAWN_PERIOD_D *6.0*mPI+mPI/2.0) +50 ;
	DrawCursor(pt_buttoneffect,1);
	pt_buttoneffect.x = 32 * cos(respawn_timer / RESPAWN_PERIOD_D *6.0*mPI+mPI+mPI/2.0)+50  ;
	pt_buttoneffect.y = 32 * sin(respawn_timer / RESPAWN_PERIOD_D *6.0*mPI+mPI+mPI/2.0) +50 ;
	DrawCursor(pt_buttoneffect,2);
	/*
	if ( respawn_timer < RESPAWN_PERIOD_NINETY_P ) {
		pt_buttoneffect.x = 32 * cos(respawn_timer / RESPAWN_PERIOD_NINETY_P *4.0*mPI+mPI/2.0) +50 ;
		pt_buttoneffect.y = 32 * sin(respawn_timer / RESPAWN_PERIOD_NINETY_P *4.0*mPI+mPI/2.0) +50 ;
		DrawCursor(pt_buttoneffect,1);
		pt_buttoneffect.x = 32 * cos(respawn_timer / RESPAWN_PERIOD_NINETY_P *6.0*mPI+mPI+mPI/2.0)+50  ;
		pt_buttoneffect.y = 32 * sin(respawn_timer / RESPAWN_PERIOD_NINETY_P *6.0*mPI+mPI+mPI/2.0) +50 ;
		DrawCursor(pt_buttoneffect,2);
	}
	else {
		pt_buttoneffect.x = 50 ;
		pt_buttoneffect.y = 32 * sin(respawn_timer / RESPAWN_PERIOD_TEN_P *mPI+mPI/2.0) +50 ;
		DrawCursor(pt_buttoneffect,1);
		pt_buttoneffect.x =  50;
		pt_buttoneffect.y = 32 * sin(respawn_timer / RESPAWN_PERIOD_TEN_P *mPI+mPI+mPI/2.0) +50 ;
		DrawCursor(pt_buttoneffect,2);
	}*/
}

////////////////////
// Draw_MiniMap
///////////////////
void Display::DrawMiniMap( Unit *u){
	SelectObject(back_dc,nullPen);
	if ( *zoom <= 5 ) 
		return;

	minimap_resolution = 10;
	int x_adj = WWAD+220;//disp_center->x+220;
	int y_adj = WHAD+130;//disp_center->y+130;
	
	for ( int i = 0 ; i <= MAX_NUM_UNIT; i++ ) {
		if ( !u[i].active  || u[i].dead ) 
			continue;

		if ( u[i].team == 0 ) 
			SparkleVar = SparkleP;
		else 
			SparkleVar = SparkleG;
		
		SelectObject(back_dc,SparkleVar[MAX_WEP_SPARKLE-1]);
		Rectangle(back_dc, u[i].x/minimap_resolution   - disp_center->x/minimap_resolution+x_adj,  
						   u[i].y/minimap_resolution-0 - disp_center->y/minimap_resolution + y_adj, 
						   u[i].x/minimap_resolution-4 - disp_center->x/minimap_resolution+x_adj,  
						   u[i].y/minimap_resolution-4 - disp_center->y/minimap_resolution+ y_adj);
	}

	//user unit
	if ( u[MAX_NUM_UNIT].userunit )
		Rectangle(back_dc,  -0+x_adj, y_adj, 
							-6+x_adj, y_adj - 6);

	for ( int i =0; i < MAXARROW; i++ ) {
		if ( disp_arw[i].active && disp_arw[i].flying 
			//&& disp_arw[i].front.x > WWAD && disp_arw[i].front.y > WHAD 
			//&& disp_arw[i].front.x < WWAD+WINDOW_WIDTH && disp_arw[i].front.y < WHAD+WINDOW_HEIGHT
			)
			SetPixel(back_dc, - disp_center->x/minimap_resolution + disp_arw[i].front.x/minimap_resolution + x_adj,
							  - disp_center->y/minimap_resolution + disp_arw[i].front.y/minimap_resolution + y_adj, RGB(255,0,0)); 
	}

	//draws rectange around the minimap to show the boundary of current screen view. 
	SelectObject(back_dc, white1Pen);
	SetBkColor(back_dc, RGB(0,0,0)); // black background
	dummy_dx = WWAD/((*zoom)/minimap_resolution);
	dummy_dy = WHAD/((*zoom)/minimap_resolution);
	pntArray[0].x = x_adj - dummy_dx; pntArray[0].y = y_adj - dummy_dy;
	pntArray[1].x = x_adj - dummy_dx; pntArray[1].y = y_adj + dummy_dy;
	pntArray[2].x = x_adj + dummy_dx; pntArray[2].y = y_adj + dummy_dy;
	pntArray[3].x = x_adj + dummy_dx; pntArray[3].y = y_adj - dummy_dy;
	pntArray[4].x = x_adj - dummy_dx; pntArray[4].y = y_adj - dummy_dy;

	Polyline(back_dc,pntArray,5);
}

#ifdef smode
/////////////////////////
//  Draws smode-related ''stuff''
////////////////////////
void Display::DrawSmode( modeSpirit *S ){

	if ( S->team == 0 ) 
		SparkleVar = SparkleP;
	else 
		SparkleVar = SparkleG;

	SelectObject( back_dc, SparkleVar[MAX_WEP_SPARKLE-1] );
	SelectObject( back_dc, nullPen);

	Rectangle(back_dc, 300, 500, 300  + S->print_charge() , 510);

}



#endif