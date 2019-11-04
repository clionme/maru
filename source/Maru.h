#pragma once

#include "resource.h"
#include "Display.h"
#include "DataTypes.h"
#include "microAI.h"
#include "macroAI.h"
#include "fieldAI.h"
#include "battleEE.h"
#include "battleFEE.h"
#include "modeSpirit.h"

// Windows Header Files:
#include <windows.h>

// C RunTime Header Files:
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define MAX_LOADSTRING 100

#define U_UNIT AL->u[MAX_NUM_UNIT] //User unit

Display Disp_back;
macroAI maAI;
fieldAI fiAI;
battleEE bEE;
battleFEE bFEE;

bool move_w, move_a, move_s, move_d; 
bool LButtonDown, RButtonDown;

bool pause;
int  State; /*  0: Title,  
				1: Battle,  
				2: Inter mission,  
				3: game over
			*/

//num_teams = 2, for now
int num_dead[MAX_NUM_TEAM];

//user unit's dummy target, its a place holder.
Unit User_dummy_target;

//WeaponData
WeaponData WD[NUM_UNIT_TYPES];

//LGroups
LGroup L[MAX_NUM_LGROUP];
LGroup *AL;

//Arrows
Arrow arw[MAXARROW]; //maximum of 100 arrows
int next_arrow_counter = 0; //allocation counter for each arrow

//Hit Effect
HitEffect hef[EFFECT_NUM];
int next_effect_counter = 0;

int respawn_timer;
bool view_center;

inline void game_pause(HWND);

inline void Maru_Loop_Main();
inline void Maru_Loop_Sub();
inline void Maru_To_Title_Screen(HWND);
inline void Maru_To_Battle_Screen(HWND, int stage);
inline void Maru_To_Next_Stage(HWND, int stage);

int game_stage;
int stage_text_counter;

inline void DrawMain();
inline void DrawTitle();
inline void DrawStageText(int);

inline void Init_GameObject();
inline void Init_PrepBattle(int stage);
inline void Init_LGroup_Units(LGroup*, 
	int, int, int, double, double);
inline void Init_SetWeapon(Unit*, int);

POINT mpt,   // adjustments for the main window position
	pt_cur,  // current cursor position
	pt_acur, // current cursor position within window axis
	pt_bcur; // cursor position within the battle field

int wheel; //mouse wheel input
double zoom;
POINT disp_center;

TCHAR text[320] = {};

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING] = "_";					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING] = "__";			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HDC fore_dc;