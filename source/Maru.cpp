// Maru.cpp : Defines the entry point for the application.
//
// Top module. 
// 
//   Creates Window and controls submodules
//
//					Dev Tool : MS VC++ 2010 Express 
//					Test Env : MS Windows 7 SP1
//
//                  Created by John J. You,    Sep ~ Oct. 2012
//     

#include "stdafx.h"
#include "Maru.h"

#ifdef smode
	modeSpirit mS(&pt_acur);
#endif

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: Place code here.
	MSG msg;

	HACCEL hAccelTable;

	// Initialize global strings
	//LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadString(hInstance, IDC_MARU, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	Init_GameObject();

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MARU));

	// Main message loop:

	while(true) 
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
			{
				// translate keystroke messages into the right format
				TranslateMessage(&msg);

				// send the message to the WindowProc function
				DispatchMessage(&msg);

				if(msg.message == WM_QUIT)
					break;
			}
		else 
		{
			//Disp_back.DrawToForeDC(fore_dc,  (U_UNIT.x )- WWAD*100/zoom  ,  (U_UNIT.y )- WHAD*100/zoom )	;
			//if ( State != 1 )
			//	Disp_back.DrawToForeDC(fore_dc)	;
			//DrawMain();
			//Disp_back.DrawToForeDC(fore_dc, u[0].x - WWAD, u[0].y - WHAD )	;
		}
	return (int) msg.wParam;
}


//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MARU));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MARU);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow( szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL );


   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;

	mpt.x = 0;
	mpt.y = 0;
	ClientToScreen(hWnd, &mpt); 
    //SetCursorPos(mpt.x, mpt.y); 
	//PAINTSTRUCT ps;

	switch (message)
	{

	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;

	case WM_CREATE:
		{
			ShowCursor(false);				

			//////////////////////
			//set display class
			//////////////////////
			fore_dc = GetDC(hWnd);

			SetTimer(hWnd, 11, 5,	NULL); //the main display timer

#ifdef debug_mode
			game_stage = 0;
			Maru_To_Next_Stage(hWnd, game_stage);		
#else
			Maru_To_Title_Screen(hWnd);
#endif
		}
		break;

	case WM_TIMER:
		{
			switch(wParam)
			{	
			case 11:
				// alternatively, we can use the position under _tWinMain,
				// but this place seems tobe better at displaying large amount of units
				Disp_back.DrawToForeDC(fore_dc)	;
				return 0;

			case 2:
				/////////////////////////////////
				// main loop is this part 
				/////////////////////////////////
				
				Maru_Loop_Main();
				
				return 0;

			case 10:

				Maru_Loop_Sub();

				for ( int k = 0; k < MAX_NUM_LGROUP; k++ ) {
					if ( L[k].team != AL->team 
						&& L[k].active 
						&& !L[k].empty )
						break;

					if ( k == MAX_NUM_LGROUP-1 )
						Maru_To_Next_Stage(hWnd, ++game_stage);
				}

				for ( int k = 0; k < MAX_NUM_LGROUP; k++ ) {
					if ( L[k].team == AL->team 
						&& L[k].active 
						&& !L[k].empty )
						break;

					if ( k == MAX_NUM_LGROUP-1 ) {
						game_stage = -1;
						Maru_To_Next_Stage(hWnd, game_stage);
					}
				}

				return 0;

			case 5: //when the game is paused or in title screen
				if ( State == 0 ) { //title
					DrawTitle();		
				}
				else if (State == 1) { //paused 
					DrawMain();		
				}
				return 0;

			case 6: //prints stage text
				stage_text_counter++;
				if ( stage_text_counter > 0 ) {
					if ( game_stage >= 0 )
						Maru_To_Battle_Screen(hWnd, game_stage);
					else 
						Maru_To_Title_Screen(hWnd);
				}
				return 0;

			case 3:
				//Walking script. 
				for ( int k = 0 ; k < MAX_NUM_LGROUP; k++ ) {
					if ( !L[k].within_battle_range ) 
						continue;

					for ( int i = 0; i <= MAX_NUM_UNIT; i ++ ) {
						if ( L[k].u[i].active && !L[k].u[i].dead) {
							if ( L[k].u[i].x != L[k].u[i].old.x || L[k].u[i].y != L[k].u[i].old.y ) {
								L[k].u[i].sequence+=1;
								L[k].u[i].sequence %= 4;
							}
							else 
								L[k].u[i].sequence = 0;
						}
					}
				}
				return 0;
			}
		}
		break;

	case WM_MOUSEMOVE:
		{
			GetCursorPos(&pt_cur);
			//pt_acur.x = pt_cur.x - mpt.x;// - WWAD; //cursor point within background axis
			//pt_acur.y = pt_cur.y - mpt.y ;//- WHAD; //cursor point within background axis
//#ifdef smode
			pt_bcur.x = (pt_cur.x- mpt.x - WWAD)*100/zoom + U_UNIT.x;
			pt_bcur.y = (pt_cur.y- mpt.y - WHAD)*100/zoom + U_UNIT.y;
//#endif
		}
		break;

	case WM_MOUSEWHEEL:
		wheel = GET_WHEEL_DELTA_WPARAM(wParam);
		
		//Other controls for mouse wheel
		// 1. stance shifter ( it is now used for Dash mode ) 
		// 2. ACT_gurad and ACT_poke of user unit ( mapped to keyboard now ) 
		 /*
			if ( U_UNIT.stamina < 30 && U_UNIT.wep.stance == 0 )
				return 0;

			 if (!U_UNIT.wep.stance_shift ) {
				 
				U_UNIT.wep.stance_shift = true;	 
				 
				if ( wheel < 0 && U_UNIT.wep.stance != 1 ) 
					U_UNIT.stamina -= 15;
				 
				else if ( wheel > 0 && U_UNIT.wep.stance != U_UNIT.wep.num_of_stance-1 ) 
					U_UNIT.stamina -= 15;

				
				if ( wheel < 0 ) {
					if ( U_UNIT.wep.stance == 0 ) 
						U_UNIT.wep.next_stance = U_UNIT.wep.num_of_stance-1;
					else 
						U_UNIT.wep.next_stance = U_UNIT.wep.stance - 1;
				}
				else if ( wheel > 0 )
					U_UNIT.wep.next_stance = (U_UNIT.wep.stance+ 1) % U_UNIT.wep.num_of_stance;
			 }	
			 */
		 
		 /*
		 if ( U_UNIT.dead ) 
			 return 0;

		 if ( wheel < 0 ) 
			 U_UNIT.ACT_type = ACT_guard;
		 else if ( wheel > 0) 
			 U_UNIT.ACT_type = ACT_poke;
			 */

		//Map zoom in / out
		if ( wheel > 0) {
			if ( zoom <= 110 && zoom > 90)
				zoom += 45; 
			else if (zoom > 5){
				zoom += 45;
			}
			else {
				zoom = 10;
			}
		}
		else if ( wheel < 0 && zoom > 30 ) {
			if ( zoom > ZOOM_M_THRESHOLD )
				zoom -= 45;
			else
				zoom -= 45;
		}
		else if ( wheel < 0 && zoom < 30 ) {
			zoom = 2;
		}

		if ( zoom <= 0.0 ) 
			zoom += 100;

		Maru_Loop_Sub();

		return 0;

	case WM_LBUTTONDOWN:

		GetCursorPos(&pt_cur);
		pt_acur.x = pt_cur.x - mpt.x;// - WWAD; //cursor point within background axis
		pt_acur.y = pt_cur.y - mpt.y ;//- WHAD; //cursor point within background axis
		
		switch(State){
		//Title screen
		case 0:
			if		(  pt_acur.x > WWAD + 100
					&& pt_acur.x < WWAD + 250 
					&& pt_acur.y > WHAD
					&& pt_acur.y < WHAD + 50 ) {
				exit(1);
			}
			else if (  pt_acur.x > WWAD + 100
					&& pt_acur.x < WWAD + 250 
					&& pt_acur.y > WHAD - 60
					&& pt_acur.y < WHAD - 10 ) {
				//Starts the game
				Maru_To_Next_Stage(hWnd, 0);
			}
			return 0;
		//Battle screen
		case 1:
			// pause button
			if (   pt_acur.x < 90 && pt_acur.x > 10 
				&& pt_acur.y < 90 && pt_acur.y > 10 ) {

					game_pause(hWnd);
			}
			//title button
			else if ( pt_acur.x < 90 && pt_acur.x > 10 
				   && pt_acur.y > 460 && pt_acur.y < 540 ) {
				Maru_To_Title_Screen(hWnd);
			}
			// Battle field is clicked
			else {
				//No action when the unit is dead
				if ( U_UNIT.dead )
					return 0;
			
				if ( zoom > ZOOM_L_THRESHOLD ) {
					if ( U_UNIT.wep.weapon_type == 3 ) {
						U_UNIT.ACT_type = ACT_bowaim;
					}
					else 
						U_UNIT.ACT_type = ACT_idle;
				}
				LButtonDown = true;
				RButtonDown = false;
			}
			return 0;
		}

	case WM_LBUTTONUP:
		if ( LButtonDown ) { 
			if ( zoom < ZOOM_L_THRESHOLD ) {
				AL->uDefaultDestination.x = pt_bcur.x;
				AL->uDefaultDestination.y = pt_bcur.y;
				AL->u[MAX_NUM_UNIT].MV_dest.x = pt_bcur.x;
				AL->u[MAX_NUM_UNIT].MV_dest.y = pt_bcur.y;
				AL->user_order_given = true;
			}
			else {
				//U_UNIT.ACT_type = ACT_idle;
				U_UNIT.ACT_type = ACT_hswing;
				if ( U_UNIT.wep.weapon_type == 3)
					U_UNIT.ACT_type = ACT_bowfire;
				else if ( U_UNIT.wep.weapon_type == 4 || U_UNIT.wep.weapon_type == 2) 
					//U_UNIT.ACT_type = ACT_dashNslash;
					U_UNIT.ACT_type = ACT_poke;
				else
					U_UNIT.ACT_type = ACT_hswing;
			}
			LButtonDown = false;
			RButtonDown = false;
		}
		return 0;

	case WM_RBUTTONDOWN:

		if ( U_UNIT.dead )
			return 0;
		/*
		if ( U_UNIT.wep.stance == 0 ) 
			U_UNIT.ACT_type = ACT_fswing;//poke;//swing;//guard;
		else if ( U_UNIT.wep.stance == 1 ) 
			U_UNIT.ACT_type = ACT_guard;
			*/
		U_UNIT.ACT_type = ACT_idle;

		LButtonDown = false;
		RButtonDown = true;
		return 0;

	case WM_RBUTTONUP:
		if ( RButtonDown) {
			
			U_UNIT.ACT_type = ACT_fswing;
			
			LButtonDown = false;
			RButtonDown = false;
		}
		return 0;
	
	case WM_KEYDOWN: 
            switch (wParam) 
            { 
                case 'A':   // Process the LEFT ARROW key. 
					move_a = true;                     
                    break; 
                case 'D':     // Process the RIGHT ARROW key. 
                     move_d = true;
                    break; 
                case 'W':     // Process the UP ARROW key. 
                     move_w = true;
                    break; 
				case 'S':      // Process the UP ARROW key. 
                     move_s = true;
                    break; 
				
				case 'Q':// Guard
					if ( U_UNIT.ACT_type == ACT_guard ) 
						U_UNIT.ACT_type = ACT_idle;
					else
						U_UNIT.ACT_type = ACT_guard;
                    break; 
				
				case 'R':      // Poke
					if ( U_UNIT.ACT_type == ACT_poke ) 
						U_UNIT.ACT_type = ACT_idle;
					else
						U_UNIT.ACT_type = ACT_poke;
                    break;

				case 'E':      // dash n sla
					if ( U_UNIT.ACT_type == ACT_dashNslash ) 
						U_UNIT.ACT_type = ACT_idle;
					else
						U_UNIT.ACT_type = ACT_dashNslash;
                    break;

				case 'P':
					view_center = !view_center;
					//U_UNIT.x = 0;
					//U_UNIT.y = 0;
					break;

				case VK_SHIFT :
					U_UNIT.roll = true;
					break;

				case VK_ESCAPE:
					game_pause(hWnd);
					break;

				case VK_F1:
					exit(1);
					break;
			}
			return 0;

	case WM_KEYUP:
	            switch (wParam) 
            { 
                case 'A':   // Process the LEFT ARROW key. 
					move_a = false;                     
                    break; 
                case 'D':     // Process the RIGHT ARROW key. 
                     move_d = false;
                    break; 
                case 'W':     // Process the UP ARROW key. 
                     move_w = false;
                    break; 
				case 'S':      // Process the UP ARROW key. 
                     move_s = false;
                    break; 

				case 'L': //kills everything
					for ( int k = 0; k < MAX_NUM_LGROUP; k++ ) {
						if ( !L[k].active || !L[k].within_battle_range )
							continue;
						for ( int i = 0 ; i <= MAX_NUM_UNIT; i++) {
							if ( L[k].u[i].active && !L[k].u[i].dead ) {
								L[k].u[i].hit = true;
								L[k].u[i].stat_currentHP = -10;
							}
						}
					}
					break;
				
				case 'K': //kills enemy
					for ( int k = 0; k < MAX_NUM_LGROUP; k++ ) {
						if ( !L[k].active || !L[k].within_battle_range )
							continue;
						if ( U_UNIT.team == L[k].team )
							continue;
						for ( int i = 0 ; i <= MAX_NUM_UNIT; i++) {
							if ( L[k].u[i].active && !L[k].u[i].dead ) {
								L[k].u[i].hit = true;
								L[k].u[i].stat_currentHP = -10;
							}
						}
					}
					break;

				case 'N':
					Maru_To_Next_Stage(hWnd, ++game_stage);
					break;

				case VK_SHIFT :
					U_UNIT.roll = false;
					
					break;
			}
			return 0;

	
	return 0;

		break;

	case WM_DESTROY:
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		KillTimer(hWnd, 4);
		KillTimer(hWnd, 5);
		Disp_back.~Display();
		
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

/////////////////////////
//pauses the game
/////////////////////////
inline void game_pause(HWND hWnd){
	if ( !pause ) {
		ShowCursor(true);
		KillTimer(hWnd, 2);
		SetTimer(hWnd, 5, 20,	NULL);
	}
	else { 
		ShowCursor(false);
		SetTimer(hWnd, 2, 20,	NULL);
		KillTimer(hWnd, 5);
	}
	pause = !pause;
}

/////////////////////////
//runs the game - sub loop
//    1. adjusts LGroup's ef(enemy - firendly group pointers) and other counters
//    2. (to be added) efficient display form
/////////////////////////
inline void Maru_Loop_Sub(){

	bFEE.FEE_Adjust_ef(L, AL);

}

/////////////////////////
//runs the game
/////////////////////////
inline void Maru_Loop_Main(){
	
#ifdef smode

	if (U_UNIT.dead) {
		mS.mode_on = true;
	}
	if (mS.mode_on)
		mS.Spirit_Main(L);

#endif

	/////////////////////////
	//  LGroup Part
	//      =field level=
	/////////////////////////
	/*
	for ( int i = 0 ; i < MAX_NUM_LGROUP; i++ ) {
		if ( L[i].active && !L[i].empty ) {
			if (!L[i].within_battle_range )
				fiAI.simple_Move_To_AL(&L[i],AL);
		}
	}
	*/
	fiAI.simple_Move_To_Closest(L);

	/////////////////////////
	//  SGroup and Units Part
	//      -macro & micro level-
	/////////////////////////

	if ( U_UNIT.MV_type == MV_to_P_march ) {
		U_UNIT.ACT_type = ACT_idle;
	}

	pt_acur.x = pt_cur.x - mpt.x ; //cursor point within background axis
	pt_acur.y = pt_cur.y - mpt.y ; //cursor point within background axis

	//Resets the targetted counter
	for ( int i = 0 ; i< MAX_NUM_LGROUP; i++ ) {
		if ( !L[i].within_battle_range ) 
			continue;
		maAI.TargetCountReset(&L[i],L[i].u); //counts for Targeted numbers are reseted.
	}

	for ( int i = 0 ; i< MAX_NUM_LGROUP; i++ ) {
		if ( !L[i].within_battle_range ) 
			continue;
		maAI.TeamControl_Simple(&L[i]);
		maAI.DeadAnimation(&L[i]);
	}

	//release userunit from march
	if ( U_UNIT.MV_type != MV_to_P_march || dist(U_UNIT,(U_UNIT.MV_dest)) < 50 
		|| move_a || move_d || move_s || move_w ) {

			if ( AL->print_e_num() != 0 )
				U_UNIT.MV_type = MV_userunit;
		//U_UNIT.group[0] = 0;
		//AL->g[U_UNIT.team].remove(&U_UNIT);
	}

	if ( U_UNIT.dead )
		U_UNIT.DA_counter--;

	//respawn timer
	if ( !pause ) {
		respawn_timer++;
		respawn_timer %= RESPAWN_PERIOD;

		//if ( respawn_timer == 0 ) 
		//	bFEE.SimpleMeta_MRespawn(L);
	}
	//used to make each action to occur only once after each input
	//can be commented out to make actions to repeat when holding down the button.
				
	if ( U_UNIT.AI_timer == AI_period - 2 ) {
		U_UNIT.ACT_type = ACT_idle;
		U_UNIT.AI_timer = 0; //reset
	}
				
	bEE.Move_A(&U_UNIT,pt_acur, move_a,move_w,move_d,move_s); //user input
	for ( int i = 0 ; i < MAX_NUM_LGROUP; i++ ) {
		if ( L[i].within_battle_range ) {
			for ( int j = 0; j < MAX_NUM_UNIT ; j++ )
				bEE.Move_M(&L[i].u[j]);
						
			for ( int j = 0 ; j < L[i].print_e_num(); j++ )
				if ( L[i].e(j)->within_battle_range )
					bEE.UnitCollisionChecker(L[i].u, L[i].e(j)->u );

			for ( int j = 0 ; j < L[i].print_f_num(); j++ )
				if ( L[i].f(j)->within_battle_range )
					bEE.UnitCollisionChecker(L[i].u, L[i].f(j)->u );					
							
			for ( int j = 0; j < MAX_NUM_UNIT ; j++ )
				bEE.Move_MW(&L[i].u[j]);
		}
	}
		
	U_UNIT.Target = &User_dummy_target;
	bEE.Move_UW(&U_UNIT, pt_acur); //user input
	bEE.Move_Arrow(arw);
	bFEE.SimpleMeta_MDead(L,num_dead);

	for ( int i = 0 ; i < MAX_NUM_LGROUP; i++ ) {
		if ( L[i].within_battle_range ) {						
			for ( int j = 0 ; j < L[i].print_e_num(); j++ ) {
				//if ( L[i].team == L[i].e(j)->team )
				//	exit(1); //for debugging
				if ( L[i].e(j)->within_battle_range )
					bEE.CollisionDetection(L[i].u, L[i].e(j)->u, arw);					
			}
		}
	}

	bEE.Chrg_Arrow(arw);

	DrawMain();	
}

///////////////////////////////////
// DrawMain
//////////////////////////////////
inline void DrawMain(){

	if ( zoom < ZOOM_L_THRESHOLD ) {
		if (view_center) {
			disp_center.x = pt_bcur.x;
			disp_center.y = pt_bcur.y;
		}
		else {
			disp_center.x = AL->center.x;
			disp_center.y = AL->center.y;
		}
	}
	else {
		disp_center.x = U_UNIT.x;
		disp_center.y = U_UNIT.y;
	}
/*
	U_UNIT.x = (int) U_UNIT.x;
	U_UNIT.y = (int) U_UNIT.y;
	disp_center.x = (int) disp_center.x;
	disp_center.y = (int) disp_center.y;
*/
	Disp_back.DrawRESET_back_dc();
#ifdef debug_mode
	Disp_back.DrawBackground();
#endif
	if ( zoom < 100 ) 
		Disp_back.DrawBackground();

	Disp_back.DrawWepReach(&U_UNIT, pt_acur, LButtonDown,RButtonDown);
	Disp_back.DrawField(L);

	Disp_back.DrawButton(pause,respawn_timer);
	Disp_back.DrawCursor(pt_acur,0); //0 : black n white cursor

	if ( zoom > ZOOM_L_THRESHOLD )
	for ( int i = 0 ; i < MAX_NUM_LGROUP; i++ ) {
		if ( L[i].active && L[i].within_battle_range ) { 
			Disp_back.DrawMiniMap( L[i].u );
		}
	}

#ifdef debug_mode		
	swprintf_s(text,L"Death");
	Disp_back.DrawText(text, WWAD - 280, WHAD - 280);

	swprintf_s(text,L"B : %d", num_dead[0]);
	Disp_back.DrawText(text, WWAD- 280, WHAD - 260);

	swprintf_s(text,L"G : %d", num_dead[1]);
	Disp_back.DrawText(text, WWAD - 280, WHAD - 240);

	swprintf_s(text, _T("ZOOM : %3.0lf %%"), zoom);
	Disp_back.DrawText(text, WWAD +200 , WHAD +150);


	//prints text messages for debugging purpose
	//Coordinates

	_stprintf_s(text,L"cha %4.1lf, %4.1lf", U_UNIT.x, U_UNIT.y);
	Disp_back.DrawText(text, 140+WWAD, -200+WHAD);
	/*
	swprintf_s(text,L"wep %4.1lf, %4.1lf", U_UNIT.wep.x, U_UNIT.wep.y);
	Disp_back.DrawText(text, 140+WWAD, -150+WHAD);

	//_stprintf_s(text,L"cur %4.1lf, %4.1lf", pt_cur.x - mpt.x + WWAD -WWAD, pt_cur.y - mpt.y + WHAD-WHAD);
	_stprintf_s(text,L"cur %d, %d", pt_bcur.x , pt_bcur.y );
	Disp_back.DrawText(text, 140+WWAD, -110+WHAD);

	_stprintf_s(text,L"ATarget %4.1lf, %d", U_UNIT.Target->x, U_UNIT.Target->team);
	Disp_back.DrawText(text, 140+WWAD, - 90+WHAD);

	_stprintf_s(text,L"angle speed %4.5lf", U_UNIT.wep.angle_speed);
	Disp_back.DrawText(text, 140+WWAD, WHAD-210);

	_stprintf_s(text,L"target %4.5lf", U_UNIT.wep.target_angle);
	Disp_back.DrawText(text, 140+WWAD, WHAD-230);
	*/
#endif

#ifdef smode
	if (mS.mode_on)
		Disp_back.DrawSmode(&mS);
#endif

}

////////////////////
// Draws Title
////////////////////
void DrawTitle(){
	disp_center.x = U_UNIT.x;
	disp_center.y = U_UNIT.y;
	pt_acur.x = pt_cur.x - mpt.x ; //cursor point within background axis
	pt_acur.y = pt_cur.y - mpt.y ; //cursor point within background axis
	Disp_back.DrawTitleLogo(pt_acur);
	Disp_back.DrawCursor( pt_acur, 0);
}

/////////////////////
// Draws stage text
/////////////////////
inline void DrawStageText(int stage) {
	Disp_back.DrawRESET_back_dc();
	/*
	if ( stage == -1 )
		_stprintf_s(text,L"GAME OVER");
	else 
		_stprintf_s(text,L"STAGE %2d", stage);
		*/
	
	Disp_back.DrawText(text, WWAD-50, WHAD-100);
}

inline void Maru_To_Next_Stage(HWND hWnd, int stage){

	DrawStageText(stage);

	KillTimer(hWnd, 3);
	KillTimer(hWnd, 5);
	KillTimer(hWnd, 2);
	KillTimer(hWnd, 10);
	SetTimer(hWnd, 6, 10, NULL );
    stage_text_counter = -50;
	
	#ifdef debug_mode
	//prints stage text
		stage_text_counter = 0;
	#endif
}

inline void Maru_To_Battle_Screen(HWND hWnd, int stage) {

	view_center = false;
	zoom = 100;
	Init_GameObject();

	//Starts the game
	Init_PrepBattle(stage);
	KillTimer(hWnd, 5);
	KillTimer(hWnd, 6);
	SetTimer(hWnd, 2, 20, NULL ) ;//20,	NULL);
	SetTimer(hWnd, 3, 250,	NULL);  // for walking sprites
	SetTimer(hWnd, 10, 2000, NULL); // for LGroup control
	pause = false;
	State = 1;	

	bEE = battleEE(&next_effect_counter, hef, &LButtonDown, &RButtonDown,arw,&next_arrow_counter);
	Disp_back = Display(fore_dc, hInst, &zoom, &disp_center, arw, hef);
	maAI = macroAI(&U_UNIT);
	respawn_timer = RESPAWN_PERIOD-1;

#ifdef debug_mode
			Disp_back.Maru_L = L;
#endif

	Maru_Loop_Sub();
	Maru_Loop_Sub();
}

inline void Maru_To_Title_Screen(HWND hWnd){
	game_stage = 0;
	zoom = 100;
	//Goes back to the title screen
	Disp_back = Display(fore_dc, hInst, &zoom, &disp_center, arw, hef);
	KillTimer(hWnd, 2);
	KillTimer(hWnd, 3);
	KillTimer(hWnd, 6);
	KillTimer(hWnd, 10);
	SetTimer(hWnd, 5, 20,	NULL);
	pause = true;
	U_UNIT.x = WINDOW_WIDTH;
	U_UNIT.y = WINDOW_HEIGHT;
	State = 0;
}

//////////////////////////////
//Initialization part
/////////////////////////////

inline void Init_GameObject(){
	for (int i =0; i < MAXARROW; i ++){
		arw[i].speed = 0.0;
		arw[i].flying = false;
		arw[i].active = false;
		arw[i].hwidth = 4.0;
		arw[i].length = 20.0;
	}

	for ( int i =0; i < EFFECT_NUM; i++ ) {
		hef[i].timer = 0;
	}

	//////////////////////////////
	//Sword 2
	//////////////////////////////
	WD[5].pull_max		= 0.4;
	WD[5].pull_speed	= 0.03;
	WD[5].poke_speed	= 0.10;
	WD[5].poke_move_distance = 4.0; //

	WD[5].max_angle_speed = 0.0; 
	WD[5].max_plane_speed = 10.0; 

	WD[5].hwidth  = 10; // half width
	WD[5].length = 80;
	WD[5].edges = 5;

	//////////////////////////////
	//Sword 1
	//////////////////////////////
	WD[1].pull_max		= 0.3;
	WD[1].pull_speed	= 0.03;
	WD[1].poke_speed	= 0.06;
	WD[1].poke_move_distance = 9.0; //

	WD[1].max_angle_speed = 0.0; 
	WD[1].max_plane_speed = 10.0; 

	WD[1].hwidth  = 15; // half width
	WD[1].length = 70;
	WD[1].edges = 3;

	//////////////////////////////
	//Spear 1
	//////////////////////////////
	WD[2].pull_max		= 0.5;
	WD[2].pull_speed	= 0.03;
	WD[2].poke_speed	= 0.035;
	WD[2].poke_move_distance = 6.0; //3.0;

	WD[2].max_angle_speed = 0.0; 
	WD[2].max_plane_speed = 20.0; 

	WD[2].hwidth  = 7; // half width
	WD[2].length = 150;
	WD[2].edges = 1;

	//////////////////////////////
	//Bow 1
	//////////////////////////////
	WD[3].pull_max		= 0.5;
	WD[3].pull_speed	= 0.07;
	WD[3].poke_speed	= 0.09;
	WD[3].poke_move_distance = 0.0; //

	WD[3].max_angle_speed = 0.0; 
	WD[3].max_plane_speed = 10.0; 

	WD[3].hwidth  = 39; // half width
	WD[3].length = 27;
	WD[3].edges = 0;

	//////////////////////////////
	//Dagger 1
	//////////////////////////////
	WD[4].pull_max		= 0.5;
	WD[4].pull_speed	= 0.05;//
	WD[4].poke_speed	= 0.1 ;//
	WD[4].poke_move_distance = 3.0; //

	WD[4].max_angle_speed = 0.0; 
	WD[4].max_plane_speed = 10.0; 

	WD[4].hwidth  = 9;// // half width
	WD[4].length = 59;
	WD[4].edges = 3;

	for ( int i = 0; i < MAX_NUM_LGROUP; i++ ) {
		L[i].LG_reset(i);
		L[i].Sel_reset();
		L[i].SG_reset();

		L[i].uDefaultDestination.wep.WD = &WD[1]; //for microAI, search 'Tfar'
		L[i].uDefaultDestination.dead = true;
		L[i].uDefaultDestination.TargetedBy = 0;
		L[i].uDefaultDestination.TargetedBy_R = 0;
	}

	//allocates user group 'AL'
	AL = &L[1];
	AL->usergroup = true;

	#ifdef smode
		mS = modeSpirit(&U_UNIT, &pt_bcur);
		mS.reset();
	#endif
}

inline void Init_PrepBattle(int stage){

	move_w = false;
	move_a = false;
	move_s = false;
	move_d = false;
	
	//arrow
	for (int i =0; i < MAXARROW; i ++){
		arw[i].speed = 0.0;
		arw[i].flying = false;
		arw[i].active = false;
	}

	//hit effect
	for ( int i = 0; i < EFFECT_NUM; i++ )
		hef[i].timer = -1;

	num_dead[1] = 0;
	num_dead[0] = 0;

	//-------
	//Common
	//-------
	for ( int k = 0; k < MAX_NUM_LGROUP; k++ ) {
		for ( int i = 0 ; i <= MAX_NUM_UNIT; i ++ ) {	
			L[k].u[i].AI_preset[1] = GACT_idle;
			L[k].u[i].AI_preset[2] = 0;

			if ( rand()%2 == 0 ) 
				L[k].u[i].MV_clock_wise = false;
			else 
				L[k].u[i].MV_clock_wise = true;

			L[k].u[i].userunit = false;
			L[k].u[i].active = false;

			bFEE.SimpleMeta_MResetUnit(&L[k].u[i]);
		}
	}

	//===============
	//LGroup setting
	//===============
	switch(stage){

	case 0:
		Init_LGroup_Units(&L[1], 1	, Unit_gsword, 0,		500, 500 );
		Init_LGroup_Units(&L[2], 1	, Unit_sword, 1,		150, 150 );

		AL->uDefaultDestination.x = 400;
		AL->uDefaultDestination.y = 400;
		
		//L[2].uDefaultDestination.x = 400;
		//L[2].uDefaultDestination.y = 400;

		U_UNIT.MV_type = MV_userunit;
		break;

	case 1:
		Init_LGroup_Units(&L[1], 1	, Unit_gsword, 0,		500, 500 );
		Init_LGroup_Units(&L[4], 1	, Unit_gsword, 1,		150, 150 );

		AL->uDefaultDestination.x = 400;
		AL->uDefaultDestination.y = 400;

		U_UNIT.MV_type = MV_userunit;
		break;

	case 2:
		Init_LGroup_Units(&L[1], 1	, Unit_gsword, 0,		500, 500 );
		Init_LGroup_Units(&L[2], 1	, Unit_dagger, 1,		150, 150 );

		AL->uDefaultDestination.x = 400;
		AL->uDefaultDestination.y = 400;

		U_UNIT.MV_type = MV_userunit;
		break;

	case 3:
		Init_LGroup_Units(&L[1], 1	, Unit_gsword, 1,		500, 500 );
		Init_LGroup_Units(&L[2], 1	, Unit_sword, 1,		150, 150 );
		Init_LGroup_Units(&L[4], 1	, Unit_dagger, 1,		150, 150 );

		//AL->uDefaultDestination.x = 250;
		//AL->uDefaultDestination.y = 250;
		
		//L[2].uDefaultDestination.x = 400;
		//L[2].uDefaultDestination.y = 400;

		U_UNIT.MV_type = MV_userunit;
		break;

	case 4:
		Init_LGroup_Units(&L[1], 1	, Unit_gsword, 1,		300, 300 );
		Init_LGroup_Units(&L[2], 1	, Unit_spear, 1,		150, 150 );
		Init_LGroup_Units(&L[4], 1	, Unit_bow, 1,		150, 150 );

		//AL->uDefaultDestination.x = 250;
		//AL->uDefaultDestination.y = 250;
		
		//L[2].uDefaultDestination.x = 400;
		//L[2].uDefaultDestination.y = 400;

		U_UNIT.MV_type = MV_userunit;
		break;

	case 6:

		/*
		Init_LGroup_Units(&L[1], 1	, Unit_dagger,	2,		100, 100 );
		//Init_LGroup_Units(&L[3], 1	, 4, 2,		100, 100 );
		Init_LGroup_Units(&L[2], 4	, Unit_bow,		3,		-150, -150 );

		AL->uDefaultDestination.x = 0;
		AL->uDefaultDestination.y = 0;
		L[2].uDefaultDestination.x = 0;
		L[2].uDefaultDestination.y = 0;

		U_UNIT.MV_type = MV_userunit;
		*/

		Init_LGroup_Units(&L[1], 0	, Unit_dagger,	40,		  00, 100 );
		Init_LGroup_Units(&L[3], 0	, Unit_dagger,	40,		1000, 100 );
		Init_LGroup_Units(&L[5], 0	, Unit_dagger,	40,		2000, 100 );
		Init_LGroup_Units(&L[7], 0	, Unit_dagger,	40,		3000, 100 );

		Init_LGroup_Units(&L[2], 0	, Unit_dagger,	40,		  00, 5100 );
		Init_LGroup_Units(&L[4], 0	, Unit_dagger,	40,		1000, 5100 );
		Init_LGroup_Units(&L[6], 0 	, Unit_dagger,	40,		2000, 5100 );
		Init_LGroup_Units(&L[8], 0	, Unit_dagger,	40,		3000, 5100 );

		AL->uDefaultDestination.x = 0;
		AL->uDefaultDestination.y = 10000;

		break;

	case 5:
		Init_LGroup_Units(&L[1],  1	, 1, 20,	0, 0 );
		Init_LGroup_Units(&L[12], 4	, 1, 20,	-600, -600 );
		Init_LGroup_Units(&L[14], 1	, Unit_gsword, 4,	-800, -800 );

		AL->uDefaultDestination.x = -1000;
		AL->uDefaultDestination.y = -1000;
		L[2].uDefaultDestination.x = 10000;
		L[2].uDefaultDestination.y = 10000;
		bFEE.FEE_Stepin(AL);
		bFEE.FEE_Stepin(&L[2]);
		
		break;

	case 7:

		Init_LGroup_Units(&L[0], 3 , 1, 50,	- 500,	 - 500);
		Init_LGroup_Units(&L[2], 1 , 4, 40,	- 700,	 - 700);
		Init_LGroup_Units(&L[4], 1 , Unit_gsword, 30,	- 600,	 - 600);
		Init_LGroup_Units(&L[14], 1 , 4, 15,		5000, 5000 );

		Init_LGroup_Units(&L[1],  0 ,  1,  50,	0, 0 );
		Init_LGroup_Units(&L[3],  3 , 3, 40,	300, 300);
		Init_LGroup_Units(&L[5],  1 , 1, 10,	1400, 1400);
		Init_LGroup_Units(&L[7],  1 , 3, 5,		1700, 1700);
		Init_LGroup_Units(&L[9],  1 , 2, 50,		-200, -200);
		Init_LGroup_Units(&L[11], 0 ,  0, 10,	2000, 2000);		
		Init_LGroup_Units(&L[17], 1 , 4, 15,		5000, -5000 );
		Init_LGroup_Units(&L[15], 1 , 4, 20,		-5000, 5000 );

		break;

	case 8:
		//Betty
		Init_LGroup_Units(&L[0], 0 , 0, 20,	- 1500,	 - 1500);
		Init_LGroup_Units(&L[2], 1, 1, 20,	- 1500,	 - 1500+400 );
		Init_LGroup_Units(&L[4], 2 , 0, 10,	- 6500,	 - 5000);
		Init_LGroup_Units(&L[6], 2 , 0, 10,	- 5500,	 - 5000);
		Init_LGroup_Units(&L[8], 0 , 0, 30,	- 13000,	 - 1000);

		//George
		Init_LGroup_Units(&L[1], 0 , 1, 50,	-6000, -6000 );
		Init_LGroup_Units(&L[3], 1 , 2, 10,	0, 0);
		Init_LGroup_Units(&L[5], 1 , 1, 10,	-1400, -1400);
		Init_LGroup_Units(&L[13], 0 , 0, 10,	2000, 2000);
		//Init_LGroup_Units(&L[11], 1 , 1, 8,	-8000, 2000 );
		
		break;
	}

	AL->within_battle_range = true;

	//-------
	//User Unit
	//-------
	Init_SetWeapon(&U_UNIT, 1);
	
	U_UNIT.active = true;
	U_UNIT.dead = false;
	U_UNIT.stat_currentHP = U_UNIT.stat_maxHP;
	U_UNIT.userunit = true;
	U_UNIT.x = AL->center.x;
	U_UNIT.y = AL->center.y;

	
	//U_UNIT.MV_type = MV_userunit;
}

// Initializes each unit of each LGroups
inline void Init_LGroup_Units(LGroup* L, 
			int mixed, int unit_type, int n_mem, double center_x, double center_y){

	L->active = true;
	L->within_battle_range = false;

	L->center.x = center_x;
	L->center.y = center_y;
	
	if ( mixed == 0 ) {
		for ( int i = 0; i < MAX_NUM_UNIT; i ++ ) 
			Init_SetWeapon(&L->u[i],rand()%5+1 ); 	
	}
	else if ( mixed == 1 ){
		for ( int i = 0; i < MAX_NUM_UNIT; i ++ ) 
			Init_SetWeapon(&L->u[i], unit_type );
	}
	else if ( mixed == 2 ) {
		for ( int i = 0; i < MAX_NUM_UNIT; i ++ ) 
			Init_SetWeapon(&L->u[i],rand()%2+1 );
	}
	else if ( mixed == 3 ) {
		for ( int i = 0; i < MAX_NUM_UNIT; i ++ ) {
			if ( i%2 == 0)
				Init_SetWeapon(&L->u[i],1 );
			else 
				Init_SetWeapon(&L->u[i],3 );
		}
	}
	else if ( mixed == 4 ) {
		for ( int i = 0; i < MAX_NUM_UNIT; i ++ ) {
			if ( i%2 == 0 )
				Init_SetWeapon(&L->u[i],2 );
			else 
				Init_SetWeapon(&L->u[i],3 );
		}
	}

	if ( n_mem > MAX_NUM_UNIT ) 
		n_mem = MAX_NUM_UNIT;

	for ( int i = 0; i < n_mem ; i++ ) {
		L->u[i].x = L->center.x;
		L->u[i].y = L->center.y;
		L->u[i].active = true;
		L->u[i].dead = false;
		L->u[i].stat_currentHP = L->u[i].stat_maxHP;
	}

	L->calc_nmember(); //without this, the numbers are not reset within LGroup class.
}

// Sets weapon
inline void Init_SetWeapon(Unit *u, int type){

	u->wep.stance_charge = 0;
	u->wep.stance_shift = false;
	u->wep.num_of_stance = 2;
	u->wep.stance = 0;

	u->wep.WD = &WD[type];
	u->wep.weapon_type = type;

	switch(type){
		case 1: //sword
			u->stat_maxHP = 500;
			u->max_speed = 5.0;
			u->stat_recover_speed = 5.0;
			break;

		case 2: //spear
			u->stat_maxHP = 500;
			u->max_speed = 4.0;
			u->stat_recover_speed = 1.0;
			break;

		case 3: //bow
			u->stat_maxHP = 200;
			u->max_speed = 5.5;
			u->stat_recover_speed = 3.0;
			break;

		case 4: //dagger
			u->stat_maxHP = 300;
			u->max_speed = 7.0;
			u->stat_recover_speed = 2.0;
			break;

		case 5: //gsword
			u->stat_maxHP = 600;
			u->max_speed = 3.5;
			u->stat_recover_speed = 1.0;
			break;
	}
}