// LanderGA.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "LanderGA.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
HWND hWnd;
CController* gController;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

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
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_LANDERGA, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LANDERGA));

	// Main message loop:
	bool bDone=true;
	CTimer t(FPS);
	t.Start();
	while (bDone)
	{
		while (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		
			{	
				if (msg.message==WM_QUIT)
				{
					bDone=false;
				}
				if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
				{
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
			}
		if (t.ReadyForNextTime() || gController->FastRander())
		{
			if (gController->GetFinished()==false)
			{
				gController->Update();
			}else
			{
				gController->ResetFitness();
			}
			
			InvalidateRect(hWnd,NULL,TRUE);
			UpdateWindow(hWnd);
		}
		
	}
	UnregisterClass(szWindowClass,hInstance);
	delete gController;
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
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LANDERGA));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_LANDERGA);
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

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPED     | 	   WS_CAPTION        |	   WS_SYSMENU ,
      GetSystemMetrics(SM_CXSCREEN)/2-WINDOW_WIDTH/2, GetSystemMetrics(SM_CYSCREEN)/2-WINDOW_HEIGHT/2,
	  WINDOW_WIDTH, WINDOW_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
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
	PAINTSTRUCT ps;
	HDC hdc;
	static int xClient;
	static int yclient;
	static HDC hdcBackBuffer;
	static HBITMAP hBackBitmap;
	static HBITMAP hOldBitmap;
	switch (message)
	{
	case WM_CREATE:
		{
			srand((unsigned)time(NULL));
			RECT rect;
			GetClientRect(hWnd,&rect);
			xClient=rect.right;
			yclient=rect.bottom;
			hdcBackBuffer=CreateCompatibleDC(NULL);
			HDC dc=GetDC(hWnd);
			hBackBitmap=CreateCompatibleBitmap(dc,xClient,yclient);
			hOldBitmap=(HBITMAP)SelectObject(hdcBackBuffer,hBackBitmap);
			ReleaseDC(hWnd,dc);
			gController=new CController;
		}
		break;
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
	case WM_KEYDOWN:
		{
			switch(wParam)
			{
			case 'B':
				gController->ToggleShowFitness();
				break;
			case 'F':
				gController->ToggleFastRander();
				break;
			case 'R':
				gController->Reset();
				break;
			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		BitBlt(hdcBackBuffer,0,0,xClient,yclient,NULL,0,0,BLACKNESS);
		gController->Rander(hdcBackBuffer);
		BitBlt(hdc,0,0,xClient,yclient,hdcBackBuffer,0,0,SRCCOPY);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		SelectObject(hdcBackBuffer,hOldBitmap);
		DeleteObject(hBackBitmap);
		DeleteDC(hdcBackBuffer);
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
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
