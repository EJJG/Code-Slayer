// Tic-Tac-Toe.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Tic-Tac-Toe.h"
#include <windowsx.h>
#include <TIME.H>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

												// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_TICTACTOE, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TICTACTOE));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TICTACTOE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	//wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.hbrBackground = (HBRUSH)(GetStockObject(GRAY_BRUSH));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_TICTACTOE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//Global variables
const int CELL_SIZE = 100;
HBRUSH hbr1, hbr2, hbr3, hbr4;
int board[4] = { 0,1,2,3 };
int playerTurn = 0;
int mode = 6;
int gameBoard[25] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
int winner = 0;
int wins[4];
int player1 = 0;
int player2 = 0;
int guest = 0;
const WCHAR * select[4];
WCHAR * szPlayer1[20];
WCHAR * szPlayer2[20];
const WCHAR szPlayer3[20] = L"AI";
int index;
HWND TextBox, SendButton;
WCHAR szInput1[MAX_PATH], szInput2[MAX_PATH];
int page = 0;
ofstream file;
/*
Chart
00-01-02-03-04
05-06-07-08-09
10-11-12-1 3-14
15-16-17-18-19
20-21-22-23-24
*/
int cells[] = { 0,1,2,3, 1,2,3,4, 5,6,7,8, 6,7,8,9, 10,11,12,13, 11,12,13,14, 15,16,17,18, 16,17,18,19, 20,21,22,23, 21,22,23,24,
0,5,10,15, 5,10,15,20, 1,6,11,16, 6,11,16,21, 2,7,12,17, 7,12,17,22, 3,8,13,18, 8,13,18,23, 4,9,14,19, 9,14,19,24,
0,6,12,18, 6,12,18,24, 1,7,13,19, 5,11,17,23, 4,8,12,16, 8,12,16,20, 9,13,17,21, 3,7,11,15 };


void time(HWND hWnd, HDC hdc)
{
	clock_t endwait;
	endwait = clock() + 1 * CLOCKS_PER_SEC;
	while (clock() < endwait) {}
}

void PlayersName(HWND hWnd, HDC hdc)
{
	 if (guest == 1)
	{
			int ret = MessageBox(hWnd, L"Will player one be playing the AI?", L"Whos playing AI?", MB_YESNO | MB_ICONQUESTION);
			if (IDNO == ret)
			{
				szPlayer1[player1] = szInput2;
				szPlayer2[player2] = szInput1;
				guest = 2;
			}
			if (IDYES == ret)
			{
				szPlayer1[player1] = szInput1;
				szPlayer2[player2] = szInput2;
				guest = 0;
			}
	}
	else
	{
		if (guest == 2)
		{
			szPlayer1[player1] = szInput2;
			szPlayer2[player2] = szInput1;
		}
		else
		{
			szPlayer1[player1] = szInput1;
			szPlayer2[player2] = szInput2;
		}
	}
}

BOOL GetGameBoard(HWND hWnd, RECT * pRect)
{

	RECT rc;
	if (GetClientRect(hWnd, &rc))
	{
		int width = rc.right - rc.left;
		int height = rc.bottom - rc.top;
		if (mode >= 6)
		{
			pRect->left = (width - CELL_SIZE * 3) / 2;
			pRect->top = (height - CELL_SIZE * 3) / 2;

			pRect->right = pRect->left + CELL_SIZE * 3;
			pRect->bottom = pRect->top + CELL_SIZE * 3;
		}
		else if (mode >= 4)
		{
			pRect->left = (width - CELL_SIZE * 4) / 2;
			pRect->top = (height - CELL_SIZE * 4) / 2;

			pRect->right = pRect->left + CELL_SIZE * 4;
			pRect->bottom = pRect->top + CELL_SIZE * 4;
		}

		else
		{

			pRect->left = (width - CELL_SIZE * 5) / 2;
			pRect->top = (height - CELL_SIZE * 5) / 2;

			pRect->right = (pRect->left + CELL_SIZE * 5);
			pRect->bottom = (pRect->top + CELL_SIZE * 5);
		}
		return TRUE;
	}

	SetRectEmpty(pRect);
	return FALSE;
}

void Drawlines(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

int GetCellNumberFromPoint(HWND hwnd, int x, int y)
{
	POINT pt = { x, y };
	RECT rc;

	if (GetGameBoard(hwnd, &rc))
	{
		if (PtInRect(&rc, pt))
		{
			//use clicks in the board
			x = pt.x - rc.left;
			y = pt.y - rc.top;

			int column = x / CELL_SIZE;
			int row = y / CELL_SIZE;

			if (mode >= 4)
			{
				int row = (y / CELL_SIZE);

				return (row);
			}
			return (column + row * 5);
		}
	}
	//outside tic-tac-toe board
	return -1;
}

BOOL GetCellRect(HWND hWnd, int index, RECT *pRect)
{
	RECT rcBoard;
	SetRectEmpty(pRect);
	if (index < 0 || index > 24)
		return FALSE;
	if (GetGameBoard(hWnd, &rcBoard))
	{
		int y, x;
		if (mode == 6)
		{
			y = index;
			x = index / 3;
		}
		else if (mode >= 4)
		{
			y = index;
			x = index / 4;
		}

		else
		{
			y = index / 5;
			x = index % 5;
		}

		if (mode == 6)
		{
			pRect->left = rcBoard.left + x * CELL_SIZE + 10;
			pRect->top = rcBoard.top + y * CELL_SIZE + 10;
			pRect->right = pRect->left + CELL_SIZE + 180;
			pRect->bottom = pRect->top + CELL_SIZE - 20;
		}

		else if (mode >= 4)
		{
			pRect->left = rcBoard.left + x * CELL_SIZE + 10;
			pRect->top = rcBoard.top + y * CELL_SIZE + 10;
			pRect->right = pRect->left + CELL_SIZE + 280;
			pRect->bottom = pRect->top + CELL_SIZE - 20;
		}
		else
		{
			pRect->left = rcBoard.left + x * CELL_SIZE + 10;
			pRect->top = rcBoard.top + y * CELL_SIZE + 10;
			pRect->right = pRect->left + CELL_SIZE - 20;
			pRect->bottom = pRect->top + CELL_SIZE - 20;
		}


		return TRUE;
	}
	return FALSE;
}
//returns the winner
int Winner(int wins[4])
{
	//row, verticals, columns

	for (int i = 0; i < ARRAYSIZE(cells); i += 4)
	{
		if (0 != gameBoard[cells[i]] && gameBoard[cells[i]] == gameBoard[cells[i + 1]] && gameBoard[cells[i]] == gameBoard[cells[i + 2]] && gameBoard[cells[i]] == gameBoard[cells[i + 3]])
		{
			wins[0] = cells[i];
			wins[1] = cells[i + 1];
			wins[2] = cells[i + 2];

			return gameBoard[cells[i]];
		}
	}

	for (int i = 0; i < ARRAYSIZE(gameBoard); ++i)
	{
		if (0 == gameBoard[i])
			return 0;
	}

	return 3;
}

void ShowTurn(HWND hWnd, HDC hdc)
{
	const WCHAR * TurnText = NULL;

	switch (winner)
	{
	case 0:
		if (mode == 0)
		{
			TurnText = (playerTurn == 1) ? szPlayer1[player1] : szPlayer2[player2];
		}
		if (mode != 0)
		{
			TurnText = (playerTurn == 1) ? szPlayer1[player1] : szPlayer3;
		}

		break;
	case 1:
		TurnText = L"Player 1 is the winnner!";
		break;
	case 2:
		TurnText = L"Player 2 is the winnner!";
		break;
	case 3:
		TurnText = L"It's a draw!";
		break;
	case 4:
		TurnText = L"AI is the winnner!";
		break;

	}
	RECT rc;
	if (NULL != TurnText && GetClientRect(hWnd, &rc))
	{
		rc.top = rc.bottom - 48;
		FillRect(hdc, &rc, (HBRUSH)GetStockObject(GRAY_BRUSH));
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);


		DrawText(hdc, TurnText, lstrlen(TurnText), &rc, DT_CENTER);
	}
}

void Score(HWND hWnd, HDC hdc)
{
	file.open("Score.txt", fstream::app);
	if (file.is_open())
	{
		if (playerTurn == 1 && winner == 1 && mode == 0)
		{

			file.write((char*)szInput1, 20);
			file << " Wins \n";
			file.write((char*)szInput2, 20);
			file << " Loses \n";
			file.close();
		}

		else if (playerTurn == 2 && winner == 2 && mode == 0)
		{
			file.write((char*)szInput2, 20);
			file << " Wins \n";
			file.write((char*)szInput1, 20);
			file << " Loses \n";
			file.close();
		}
		else if (playerTurn == 1 && winner == 1 && mode >= 1)
		{
			file.write((char*)szInput1, 20);
			file << " Wins \n";
			file.write((char*)szPlayer3, 20);
			file << " Loses \n";
			file.close();
		}
		else if (playerTurn == 4 && winner == 4 && mode >= 1)
		{
			file.write((char*)szPlayer3, 20);
			file << " Wins \n";
			file.write((char*)szInput1, 20);
			file << " Loses \n";
			file.close();
		}
		
		else if ( winner == 3)
		{
			file.write((char*)szInput2, 20);
			file << " Tie \n";
			file.write((char*)szInput1, 20);
			file << " Tie \n";
			file.close();
		}

	}
}
//Player and AI choice
void Moves(HWND hWnd, HDC hdc, int xPos, int yPos)
{
	if (mode == 0)
	{
		//calling function for button
		index = GetCellNumberFromPoint(hWnd, xPos, yPos);

		//print out Value of cell clicked on
		if (NULL != hdc)
		{
			/*WCHAR temp[100];
			wsprintf(temp, L"Index = %d", index);
			TextOut(hdc, xPos, yPos, temp, lstrlen(temp));*/
			//get cell demensions
			if (index != -1)
			{
				RECT rcCell;

				//cell is taken no other player can take over
				if ((0 == gameBoard[index]) && GetCellRect(hWnd, index, &rcCell))
				{
					gameBoard[index] = playerTurn;
					FillRect(hdc, &rcCell, (playerTurn == 2) ? hbr2 : hbr1);
					//FillRect(hdc, &rcCell, hbr1);
					winner = Winner(wins);
					if (winner == 1 || winner == 2)
					{
						MessageBox(hWnd, (winner == 1) ? L"Player 1 is the winnner!" : L"Player 2 is the winner!", L"You Win!", MB_OK | MB_ICONINFORMATION);
						Score(hWnd, hdc);
					playerTurn = 0;
					}

					else if (3 == winner)
					{
						MessageBox(hWnd, L"No one wins!", L"It's a draw", MB_OK | MB_ICONEXCLAMATION);
						Score(hWnd, hdc);
						playerTurn = 0;
					}

					else if (0 == winner)
					{
						playerTurn = (playerTurn == 1) ? 2 : 1;

					}

					ShowTurn(hWnd, hdc);
				}

			}
			ReleaseDC(hWnd, hdc);

		}
	}
}

void Move(HWND hWnd, HDC hdc, int xPos, int yPos)
{
	if (mode >= 1 && playerTurn == 1)
	{
		//calling function for button
		index = GetCellNumberFromPoint(hWnd, xPos, yPos);

		//print out Value of cell clicked on
		if (NULL != hdc)
		{
			/*WCHAR temp[100];
			wsprintf(temp, L"Index = %d", index);
			TextOut(hdc, xPos, yPos, temp, lstrlen(temp));*/
			//get cell demensions
			if (index != -1)
			{
				RECT rcCell;

				//cell is taken no other player can take over
				if ((0 == gameBoard[index]) && GetCellRect(hWnd, index, &rcCell))
				{
					gameBoard[index] = playerTurn;
					FillRect(hdc, &rcCell, hbr1);

					winner = Winner(wins);
					if (winner == 1)
					{
						MessageBox(hWnd, L"Player 1 is the winnner!", L"You Win!", MB_OK | MB_ICONINFORMATION);
						Score(hWnd, hdc);
						playerTurn = 0;

					}

					else if (3 == winner)
					{
						MessageBox(hWnd, L"No one wins!", L"It's a draw", MB_OK | MB_ICONEXCLAMATION);
						Score(hWnd, hdc);
						playerTurn = 0;
					}

					else if (0 == winner)
					{
						playerTurn = (playerTurn == 1) ? 4 : 1;

					}

					ShowTurn(hWnd, hdc);
				}

			}
			ReleaseDC(hWnd, hdc);

		}
	}
}

int modes(int &index)
{
	//stoped here!
	int winner = 25;
	int loser1 = 25;
	int loser2 = 25;
	int opt1 = 25;
	int opt2 = 25;

	for (int i = 0; i < ARRAYSIZE(cells); i += 4)
	{


		if (mode > 0)
		{
			if (mode > 1)
			{
				if (mode > 2)
				{
					//block hard
					if (1 == gameBoard[cells[i]] && 1 == gameBoard[cells[i + 1]] && 0 == gameBoard[cells[i + 2]] && 0 == gameBoard[cells[i + 3]])
						loser2 = cells[i + 2];

					else if (1 == gameBoard[cells[i]] && 1 == gameBoard[cells[i + 2]] && 0 == gameBoard[cells[i + 1]] && 0 == gameBoard[cells[i + 3]])
						loser2 = cells[i + 1];

					else if (1 == gameBoard[cells[i]] && 1 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i + 2]] && 0 == gameBoard[cells[i + 1]])
						loser2 = cells[i + 2];

					else if (1 == gameBoard[cells[i + 1]] && 1 == gameBoard[cells[i + 2]] && 0 == gameBoard[cells[i]] && 0 == gameBoard[cells[i + 3]])
						loser2 = cells[i];

					else if (1 == gameBoard[cells[i + 1]] && 1 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i]] && 0 == gameBoard[cells[i + 2]])
						loser2 = cells[i];

					else if (1 == gameBoard[cells[i + 2]] && 1 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i]] && 0 == gameBoard[cells[i + 1]])
						loser2 = cells[i];
				}
				//block medium
				if (1 == gameBoard[cells[i]] && 1 == gameBoard[cells[i + 2]] && 1 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i + 1]])

					loser1 = cells[i + 1];


				else if (1 == gameBoard[cells[i]] && 1 == gameBoard[cells[i + 1]] && 1 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i + 2]])
					loser1 = cells[i + 2];

				else if (1 == gameBoard[cells[i]] && 1 == gameBoard[cells[i + 1]] && 1 == gameBoard[cells[i + 2]] && 0 == gameBoard[cells[i + 3]])
					loser1 = cells[i + 3];

				else if (1 == gameBoard[cells[i + 1]] && 1 == gameBoard[cells[i + 2]] && 1 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i]])
					loser1 = cells[i];

				//medium options
				else if (4 == gameBoard[cells[i]] && 0 == gameBoard[cells[i + 2]] && 0 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i + 1]])
					opt2 = cells[i + 1];

				else if (0 == gameBoard[cells[i]] && 0 == gameBoard[cells[i + 2]] && 0 == gameBoard[cells[i + 3]] && 4 == gameBoard[cells[i + 1]])
					opt2 = cells[i];

				else if (0 == gameBoard[cells[i]] && 4 == gameBoard[cells[i + 2]] && 0 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i + 1]])
					opt2 = cells[i];

				else if (0 == gameBoard[cells[i]] && 0 == gameBoard[cells[i + 2]] && 4 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i + 1]])
					opt2 = cells[i];

				//options to process to win
				else if (4 == gameBoard[cells[i]] && 4 == gameBoard[cells[i + 1]] && 0 == gameBoard[cells[i + 2]] && 1 != gameBoard[cells[i + 3]])
					opt1 = cells[i + 2];

				else if (4 == gameBoard[cells[i]] && 4 == gameBoard[cells[i + 2]] && 0 == gameBoard[cells[i + 1]] && 1 != gameBoard[cells[i + 3]])
					opt1 = cells[i + 1];

				else if (4 == gameBoard[cells[i]] && 4 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i + 2]] && 1 != gameBoard[cells[i + 1]])
					opt1 = cells[i + 2];

				else if (4 == gameBoard[cells[i + 1]] && 4 == gameBoard[cells[i + 2]] && 0 == gameBoard[cells[i]] && 1 != gameBoard[cells[i + 3]])
					opt1 = cells[i];

				else if (4 == gameBoard[cells[i + 1]] && 4 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i]] && 1 != gameBoard[cells[i + 2]])
					opt1 = cells[i];

				else if (4 == gameBoard[cells[i + 2]] && 4 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i]] && 1 != gameBoard[cells[i + 1]])
					opt1 = cells[i];

			}
			//win
			if (4 == gameBoard[cells[i]] && 4 == gameBoard[cells[i + 2]] && 4 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i + 1]])

				winner = cells[i + 1];


			else if (4 == gameBoard[cells[i]] && 4 == gameBoard[cells[i + 1]] && 4 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i + 2]])

				winner = cells[i + 2];


			else if (4 == gameBoard[cells[i]] && 4 == gameBoard[cells[i + 1]] && 4 == gameBoard[cells[i + 2]] && 0 == gameBoard[cells[i + 3]])

				winner = cells[i + 3];


			else if (4 == gameBoard[cells[i + 1]] && 4 == gameBoard[cells[i + 2]] && 4 == gameBoard[cells[i + 3]] && 0 == gameBoard[cells[i]])

				winner = cells[i];
			// easy options
			else
			{
				opt2 = rand() % 25;
				while (1 == gameBoard[opt2] || 4 == gameBoard[opt2])
					opt2 = rand() % 25;
			}
		}




	}

	if (winner != 25)
		index = winner;
	else if (loser1 != 25)
		index = loser1;
	else if (loser2 != 25)
		index = loser2;
	else if (opt1 != 25)
		index = opt1;
	else if (opt2 != 25)
		index = opt2;

	//working on this
	return index;

}

void PCMoves(HWND hWnd, HDC hdc)
{
	if (4 == playerTurn)
	{
		//calling function for button

		//print out Value of cell clicked on
		HDC hdc = GetDC(hWnd);
		if (NULL != hdc)
		{
			/*WCHAR temp[100];
			wsprintf(temp, L"Index = %d", index);
			TextOut(hdc, xPos, yPos, temp, lstrlen(temp));*/
			//get cell demensions
			RECT rcCell;

			//AI
			modes(index);




			//cell is taken no other player can take over
			if ((0 == gameBoard[index]) && GetCellRect(hWnd, index, &rcCell))
			{
				gameBoard[index] = playerTurn;
				FillRect(hdc, &rcCell, hbr3);

				winner = Winner(wins);
				if (winner == 4)
				{
					MessageBox(hWnd, L"AI is the winner!", L"You Lose!", MB_OK | MB_ICONINFORMATION);
					Score(hWnd, hdc);
					playerTurn = 0;

				}

				else if (3 == winner)
				{
					MessageBox(hWnd, L"No one wins!", L"It's a draw", MB_OK | MB_ICONEXCLAMATION);
					Score(hWnd, hdc);
					playerTurn = 0;
				}

				else if (0 == winner)
				{
					playerTurn = (playerTurn == 1) ? 4 : 1;

				}

				ShowTurn(hWnd, hdc);
			}

		}
		ReleaseDC(hWnd, hdc);
	}
}

void Horizon(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

void reset(HWND hWnd, HDC hdc)
{
	if (mode >= 4)
		playerTurn = 0;
	else
	{
		int ans = MessageBox(hWnd, L"Would you like to make the first move?", L"Player", MB_YESNO | MB_ICONQUESTION);
		if (IDNO == ans)
		{
			if (mode == 0)
				playerTurn = 2;

			else
				playerTurn = 4;

		}
		else
			playerTurn = 1;
	}

	winner = 0;
	ZeroMemory(gameBoard, sizeof(gameBoard));
	InvalidateRect(hWnd, NULL, TRUE);
	UpdateWindow(hWnd);

	if (mode > 0)
	{
		PCMoves(hWnd, hdc);
	}
	PlayersName(hWnd, hdc);
}

void Register(HWND hWnd, HDC hdc)
{
	if (mode >= 5)
	{
		if (page == 2)
		{
			TextBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"Guest1",
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
				CW_USEDEFAULT, CW_USEDEFAULT, 200, 24,	// x, y, w, h
				hWnd, (HMENU)(101),
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
			GetWindowText(GetDlgItem(hWnd, 101), szInput1, MAX_PATH);
			DestroyWindow(TextBox);
			page = 0;
		}
		else if (page == 1)
		{
			TextBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"Guest2",
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
				CW_USEDEFAULT, CW_USEDEFAULT, 200, 24,	// x, y, w, h
				hWnd, (HMENU)(101),
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);
			GetWindowText(GetDlgItem(hWnd, 101), szInput2, MAX_PATH);
			DestroyWindow(TextBox);
			page = 0;
		}

		else
		{
			TextBox = CreateWindowEx(WS_EX_CLIENTEDGE, L"edit", L"",
				WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_LEFT,
				CW_USEDEFAULT, CW_USEDEFAULT, 200, 24,	// x, y, w, h
				hWnd, (HMENU)(101),
				(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE), NULL);

			if (mode == 6)
			{
				SendButton = CreateWindowEx(BS_PUSHBUTTON, L"Button", L"Enter",
					WS_CHILD | WS_VISIBLE | WS_BORDER,
					200, 34, 200, 34,
					hWnd, (HMENU)1, NULL, NULL);
				page = 1;
			}
			else if (mode == 5)
			{
				SendButton = CreateWindowEx(BS_PUSHBUTTON, L"Button", L"Enter",
					WS_CHILD | WS_VISIBLE | WS_BORDER,
					200, 34, 200, 34,
					hWnd, (HMENU)2, NULL, NULL);
				page = 1;
			}
			
		}
	}
}

void selection(HWND hWnd, HDC hdc, int xPos, int yPos)
{
	RECT rcCell;

	index = GetCellNumberFromPoint(hWnd, xPos, yPos);

	if (NULL != hdc)
	{
		if ((0 == board[index]) && GetCellRect(hWnd, index, &rcCell))
		{

			FillRect(hdc, &rcCell, hbr1);
			time(hWnd, hdc);
			if (mode == 6)
			{
				Register(hWnd, hdc);

			}

			else if (mode == 5)
			{

				int ret = MessageBox(hWnd, L"Player 2 would you like to Sing In?", L"Sing In", MB_YESNO | MB_ICONQUESTION);
				if (IDYES == ret)
				{
					Register(hWnd, hdc);
					mode = 5;
				}
				else
				{
					page = 1;
					Register(hWnd, hdc);
					mode = 0;
				}

			}
			else
			{
				mode = 1;
			}

			reset(hWnd, hdc);
		}
		else if ((1 == board[index]) && GetCellRect(hWnd, index, &rcCell))
		{

			FillRect(hdc, &rcCell, hbr2);
			time(hWnd, hdc);
			if (mode == 6)
			{
				page = 2;
				Register(hWnd, hdc);
				mode = 5;

			}
			else if (mode == 5)
				mode = 4;

			else
				mode = 2;
			reset(hWnd, hdc);


		}
		else if ((2 == board[index]) && GetCellRect(hWnd, index, &rcCell))
		{

			FillRect(hdc, &rcCell, hbr4);
			time(hWnd, hdc);
			if (mode == 6)
				DestroyWindow(hWnd);
			else if (mode == 5)
			{
				system("Score.txt");
			}
			else if (mode == 4)
				mode = 3;
			reset(hWnd, hdc);
		}
		else if ((3 == board[index]) && GetCellRect(hWnd, index, &rcCell))
		{

			FillRect(hdc, &rcCell, hbr3);
			time(hWnd, hdc);
			if (mode == 5)
				mode = 6;

			else
				mode = 5;
			reset(hWnd, hdc);

		}

	}
}

BOOL warning(HWND  hWnd, HDC hdc, int &x)
{
	file.open("Score.txt", fstream::app);
	if (playerTurn == 1)
	{
		int ret = MessageBox(hWnd, L"Will be consider the loser", L"Player 1", MB_YESNO | MB_ICONWARNING);
		if (IDYES == ret)
		{
			if (playerTurn == 2)
			{
				file.write((char*)szInput2, 20);
				file << " Wins \n";
			}
			else
			{
				file.write((char*)szPlayer3, 20);
				file << " Wins \n";
			}
			file.write((char*)szInput1, 20);
			file << " Loses \n";
			file.close();

			x = TRUE;

			return x;

		}
		else
		{
			file.close();
			x = FALSE;

			return x;
		}

	}
	else if (playerTurn == 2)
	{
		int ret = MessageBox(hWnd, L"Will be consider the loser", L"Player 2", MB_YESNO | MB_ICONWARNING);
		if (IDYES == ret)
		{
			file.write((char*)szInput1, 20);
			file << " Wins \n";
			file.write((char*)szInput2, 20);
			file << " Loses \n";
			file.close();
			x = TRUE;
			return x;
		}
		else
		{
			file.close();
			x = FALSE;
			return x;
		}
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x;
	switch (message)
	{
		//create a brush to user as player
	case WM_CREATE:
	{
		hbr1 = CreateSolidBrush(RGB(255, 0, 0));
		hbr2 = CreateSolidBrush(RGB(0, 0, 255));
		hbr3 = CreateSolidBrush(RGB(0, 225, 0));
		hbr4 = CreateSolidBrush(RGB(200, 200, 200));

	}
	case WM_COMMAND:
	{
		HDC hdc = GetDC(hWnd);
		int wmId = LOWORD(wParam);

		switch (LOWORD(wParam))
		{
		case 1:

			// Obtains input from the textbox and puts it into the char array
			GetWindowText(GetDlgItem(hWnd, 101), szInput1, MAX_PATH);
			DestroyWindow(TextBox);
			DestroyWindow(SendButton);
			mode = 5;
			page = 0;
			reset(hWnd, hdc);

			break;
		case 2:

			// Obtains input from the textbox and puts it into the char array
			GetWindowText(GetDlgItem(hWnd, 101), szInput2, MAX_PATH);
			DestroyWindow(TextBox);
			DestroyWindow(SendButton);
			mode = 0;
			page = 0;
			reset(hWnd, hdc);

			break;
		}


		// Parse the menu selections:
		switch (wmId)
		{
		case ID_SCORE:
		{
			system("Score.txt");
				
		}
		break;
		case ID_NEWGAME:
		{
			if (page == 0)
			{
				int ret = MessageBox(hWnd, L"Are you sure you want to start a new game?", L"New Game", MB_YESNO | MB_ICONQUESTION);
				if (IDYES == ret)
				{
					if (winner == 0 && mode < 4)
					{

						warning(hWnd, hdc, x);
						if (x == TRUE)
						{
							mode = 0;
							reset(hWnd, hdc);
						}
					}

					else
					{
						mode = 0;
						reset(hWnd, hdc);
					}
				}
			}
		}
		break;
		case ID_SINGLEPLAYER_EASY:
		{
			if (page == 0)
			{
				int ret = MessageBox(hWnd, L"Are you sure you want to start a new game?", L"New Game", MB_YESNO | MB_ICONQUESTION);
				if (IDYES == ret)
				{
					if (mode == 0)
						guest = 1;

					if (winner == 0 && mode < 4)
					{

						warning(hWnd, hdc, x);
						if (x == TRUE)
						{
							mode = 1;
							reset(hWnd, hdc);
						}
					}

					else
					{
						mode = 1;
						reset(hWnd, hdc);
					}
				
				}
			}
		}
		break;
		case ID_SINGLEPLAYER_MEDIUM:
		{
			if (page == 0)
			{
				int ret = MessageBox(hWnd, L"Are you sure you want to start a new game?", L"New Game", MB_YESNO | MB_ICONQUESTION);
				if (IDYES == ret)
				{
					if (mode == 0)
						guest = 1;
					if (winner == 0 && mode < 4)
					{

						warning(hWnd, hdc, x);
						if (x == TRUE)
						{
							mode = 2;
							reset(hWnd, hdc);
						}
					}

					else
					{
						mode = 2;
						reset(hWnd, hdc);
					}
				}
			}
		}
		break;
		case ID_SINGLEPLAYER_HARD:
		{
			if (page == 0)
			{
				int ret = MessageBox(hWnd, L"Are you sure you want to start a new game?", L"New Game", MB_YESNO | MB_ICONQUESTION);
				if (IDYES == ret)
				{
					if (mode == 0)
						guest = 1;
					if (winner == 0 && mode < 4)
					{

						warning(hWnd, hdc, x);
						if (x == TRUE)
						{
							mode = 3;
							reset(hWnd, hdc);
						}
					}

					else
					{
						mode = 3;
						reset(hWnd, hdc);
					}

				}
			}
		}
		break;
		case ID_MAINMENU:
		{
			if (page == 0)
			{
				int ret = MessageBox(hWnd, L"Are you sure you want to go to the Main Menu?", L"Main Menu", MB_YESNO | MB_ICONQUESTION);
				if (IDYES == ret)
				{
					if (winner == 0 && mode < 4)
					{

						warning(hWnd, hdc, x);
						if (x == TRUE)
						{
							mode = 6;
							reset(hWnd, hdc);
						}
					}

					else
					{
						mode = 6;
						reset(hWnd, hdc);
					}


				}
			}
		}
		break;		
		
		case IDM_ABOUT:
			if (page == 0)
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			}
			break;
		case IDM_EXIT:
			if (page == 0)
			{
				if (winner == 0 && mode < 4)
				{

					warning(hWnd, hdc, x);
					if (x == TRUE)
					{
						
						DestroyWindow(hWnd);
					}
				}

				else
				{
					DestroyWindow(hWnd);
				}
			}
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;

	case WM_LBUTTONDOWN:
	{

		
		if (page == 0)
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			HDC hdc = GetDC(hWnd);
			if (mode >= 4)
				selection(hWnd, hdc, xPos, yPos);

			else if (mode >= 1)
			{
				Move(hWnd, hdc, xPos, yPos);
			}
			else
			{
				Moves(hWnd, hdc, xPos, yPos);

			}
		}
	}
	break;
	case WM_LBUTTONUP:
	{
		if (page == 0)
		{
			int xPos = GET_X_LPARAM(lParam);
			int yPos = GET_Y_LPARAM(lParam);
			HDC hdc = GetDC(hWnd);


			PCMoves(hWnd, hdc);
		}

	}
	break;
	case WM_GETMINMAXINFO:
	{
		MINMAXINFO * pMinMax = (MINMAXINFO*)lParam;
		pMinMax->ptMinTrackSize.x = (CELL_SIZE * 7);
		pMinMax->ptMinTrackSize.y = (CELL_SIZE * 7);
	}
	case WM_PAINT:
	{
	
		
		
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		RECT rc;
		if (mode >= 4)
		{

			if (GetGameBoard(hWnd, &rc))
			{
				Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			}
			for (int i = 0; i < 4; i++)
				Horizon(hdc, rc.left, rc.top + CELL_SIZE *i, rc.right, rc.top + CELL_SIZE *i);

			if (mode == 6)
			{
				select[1] = L"Sing In";
				select[2] = L"Play as guest";
				select[3] = L"Exit Game";
				SetTextColor(hdc, RGB(0, 0, 0));
				TextOut(hdc, rc.left + CELL_SIZE * 1, rc.top + CELL_SIZE * .5, select[1], lstrlen(select[1]));
				TextOut(hdc, rc.left + CELL_SIZE * 1, rc.top + CELL_SIZE * 1.5, select[2], lstrlen(select[2]));
				TextOut(hdc, rc.left + CELL_SIZE * 1, rc.top + CELL_SIZE * 2.5, select[3], lstrlen(select[3]));
			}
			if (mode == 5)
			{
				select[1] = L"Multi-Player";
				select[2] = L"Single Player";
				select[3] = L"Score Player";
				select[4] = L"Register";
				SetTextColor(hdc, RGB(0, 0, 0));
				TextOut(hdc, rc.left + CELL_SIZE *1.5, rc.top + CELL_SIZE * .5, select[1], lstrlen(select[1]));
				TextOut(hdc, rc.left + CELL_SIZE *1.5, rc.top + CELL_SIZE * 1.5, select[2], lstrlen(select[2]));
				TextOut(hdc, rc.left + CELL_SIZE *1.5, rc.top + CELL_SIZE * 2.5, select[3], lstrlen(select[3]));
				TextOut(hdc, rc.left + CELL_SIZE *1.5, rc.top + CELL_SIZE * 3.5, select[4], lstrlen(select[4]));
			}
			if (mode == 4)
			{
				select[1] = L"Easy";
				select[2] = L"Medium";
				select[3] = L"Hard";
				select[4] = L"Main Menu";
				SetTextColor(hdc, RGB(0, 0, 0));
				TextOut(hdc, rc.left + CELL_SIZE *1.5, rc.top + CELL_SIZE * .5, select[1], lstrlen(select[1]));
				TextOut(hdc, rc.left + CELL_SIZE *1.5, rc.top + CELL_SIZE * 1.5, select[2], lstrlen(select[2]));
				TextOut(hdc, rc.left + CELL_SIZE *1.5, rc.top + CELL_SIZE * 2.5, select[3], lstrlen(select[3]));
				TextOut(hdc, rc.left + CELL_SIZE *1.5, rc.top + CELL_SIZE * 3.5, select[4], lstrlen(select[4]));
			}
		}

		else
		{
			if (GetGameBoard(hWnd, &rc))
			{
				RECT rcClient;
				if (GetClientRect(hWnd, &rcClient))
				{
					//display Player's name
					SetBkMode(hdc, TRANSPARENT);

					if (mode > 0)
					{
						SetTextColor(hdc, RGB(255, 0, 0));
						TextOut(hdc, 18, 20, szPlayer1[player1], lstrlen(szPlayer1[player1]));
						SetTextColor(hdc, RGB(0, 255, 0));
						TextOut(hdc, rcClient.right - 80, 20, szPlayer3, ARRAYSIZE(szPlayer3));
					}
					if (mode == 0)
					{
						SetTextColor(hdc, RGB(255, 0, 0));
						TextOut(hdc, 18, 20, szPlayer1[player1], lstrlen(szPlayer1[player1]));
						SetTextColor(hdc, RGB(0, 0, 255));
						TextOut(hdc, rcClient.right - 80, 20, szPlayer2[player2], lstrlen(szPlayer2[player2]));
					}

					ShowTurn(hWnd, hdc);
				}
				//Remove boarders
				FillRect(hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));
				//Rectangle(hdc, rc.left, rc.top, rc.right, rc.bottom);

			}

			for (int i = 1; i < 5; ++i)
			{

				//vertical lines
				Drawlines(hdc, rc.left + CELL_SIZE * i, rc.top, rc.left + CELL_SIZE *i, rc.bottom);

				//Horizantal line
				Drawlines(hdc, rc.left, rc.top + CELL_SIZE * i, rc.right, rc.top + CELL_SIZE * i);
			}
			for (int i = 0; i < 25; i++)
			{
				RECT rcCell;
				//action stays after re-size page
				if (0 != gameBoard[i] && GetCellRect(hWnd, i, &rcCell) && mode == 0)
				{

					FillRect(hdc, &rcCell, (gameBoard[i] == 2) ? hbr2 : hbr1);

				}
				else if (0 != gameBoard[i] && GetCellRect(hWnd, i, &rcCell) && mode > 0)
				{
					FillRect(hdc, &rcCell, (gameBoard[i] == 4) ? hbr3 : hbr1);
				}

			}
		}

		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		DeleteObject(hbr1);
		DeleteObject(hbr2);
		DeleteObject(hbr3);
		DeleteObject(hbr4);
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