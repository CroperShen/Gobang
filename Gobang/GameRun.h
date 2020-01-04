#pragma once
#include <Windows.h>
#include <string>
#include "ChessBoard.h"
#include <time.h>

struct Chsopt;
struct Chsopt   //储存操作
{
	int id;
	int x;
	int y;
	Chsopt() {};
};

HWND& CurrentHwnd();
Chess& CurrentGame();
void ChessPrint();
Chsopt TranslateClick(LPARAM lparam);
void SendClick(LPARAM lparam);
void SendChsopt(Chsopt opt);
Chsopt GetHumOpt();
void AnnounceWin(int i);
void ChessMain();

#include <queue>
#include "ChessAI.h"
#include "ChessAI2.h"

std::queue<Chsopt> Chsque;
bool QueLocked = false;

inline HWND & CurrentHwnd()
{
	static HWND hwnd;
	return hwnd;
}
inline Chess & CurrentGame()
{
	static Chess chess = Chess::Chess();
	return chess;
}


void ChessPrint()
{
	static ChessGame game0;
	Chess &chess = CurrentGame();
	HDC hdc = GetDC(CurrentHwnd());
	for (int i = 0; i < game0.cx; i++) for (int j = 0; j < game0.cy; j++)
	{
		if (chess.game.cell[i][j] != game0.cell[i][j])
		{
			chess.print(hdc, i, j);
			game0.cell[i][j] = chess.game.cell[i][j];
		}
	}
	ReleaseDC(CurrentHwnd(), hdc);
}
Chsopt TranslateClick(LPARAM lparam)
{
	Chsopt opt;
	ChessBoard& board = CurrentGame().board;
	ChessGame& game = CurrentGame().game;
	int x = LOWORD(lparam);
	int y = HIWORD(lparam);
	opt.x = board.GetPixLineX(x);
	opt.y = board.GetPixLineY(y);
	if ((opt.x >= 0) && (opt.x < game.cx) && (opt.y >= 0) && (opt.y < game.cy)) opt.id = 1;
	return opt;
}
void SendClick(LPARAM lparam)
{
	Chsopt opt = TranslateClick(lparam);
	SendChsopt(opt);
}
void SendChsopt(Chsopt opt)
{
	while (QueLocked == true) Sleep(1);
	QueLocked = true;
	Chsque.push(opt);
	QueLocked = false;
}

Chsopt GetHumOpt()
{
	while (Chsque.empty() || QueLocked) Sleep(10);
	QueLocked = true;
	Chsopt opt = Chsque.front();
	Chsque.pop();
	QueLocked = false;
	return opt;
}
void AnnounceWin(int i)
{
	std::string str;
	if (i == 1) str = TEXT("黑棋胜利");
	if (i == 2)str = TEXT("白棋胜利");
	MessageBox(CurrentHwnd(), TEXT(str.c_str()), NULL, MB_ICONHAND);
}

void Translate(Chess& chess, Chsopt& opt)
{
	int x;
	int y;
	if (opt.id == 0)
	{
		x = chess.board.GetPixLineX(opt.x);
		y = chess.board.GetPixLineY(opt.y);
		if ((x >= 0) && (x < chess.game.cx) && (y >= 0) && (y < chess.game.cy))
		{
			opt.x = x;
			opt.y = y;
		}
		else
		{
			opt.id = -1;
		}
	}
}

inline Chsopt GetAIOpt(ptype t,int aitype=1)
{
	switch (aitype)
	{
	case 1:
		return AIMap1::ChessAIMain(CurrentGame().game, t);
	case 2:
		return AIMap2::ChessAIMain(CurrentGame().game, t);
	}
	return AIMap1::ChessAIMain(CurrentGame().game, t);
}

void ChessMain()
{
	Chess& chess = CurrentGame();
	Chsopt opt;
	HDC hdc;
	chess.Clear();
	ChessPrint();

	int AItest_Turn = 1;
	int AIWin[3] = { 0,0,0 };

	static long time[3];
	static long num[3]; 
	static long longest[3];
	do
	{
		if (chess.Turn == 1)
		{
			opt = GetHumOpt();

		}
		else
		{
			opt = GetAIOpt(t_white,1);
		}
	
		switch (opt.id)
		{
		case 1:
			if (chess.hasWin != 0)
			{
				chess.Clear();
				ChessPrint();
				break;
			}
			if (chess.Turn == 1)
			{
				chess.PutBlack(opt.x, opt.y);
				ChessPrint();
			}
			else
			{
				chess.PutWhite(opt.x, opt.y);
				ChessPrint();
			}
			chess.judge();
			if (chess.hasWin > 0)
			{
				//AnnounceWin(chess.hasWin);
				int ai = (AItest_Turn == chess.hasWin) ? 1 : 2;
				AIWin[ai]++;
				hdc = GetDC(CurrentHwnd());
				std::string sz = "";
				sz+="AI1获胜次数：";
				sz+=std::to_string(AIWin[1]);
				sz+= "平均每步时长：";
				//sz += std::to_string(time[1]*1000/num[1]);
				sz += "最长耗费时间：";
				sz += std::to_string(longest[1]); 
				TextOut(hdc, 800, 600, TEXT(sz.c_str()), sz.size());

				sz = " ";
				sz+="AI2获胜次数：";
				sz+=std::to_string(AIWin[2]);
				sz += "平均每步时长：";
				//sz += std::to_string(time[2]*1000 / num[2]);
				sz += "最长耗费时间：";
				sz += std::to_string(longest[2]);
				TextOut(hdc, 800, 630, TEXT(sz.c_str()), sz.size());

				longest[1] = 0;
				longest[2] = 0;
				ReleaseDC(CurrentHwnd(), hdc);
				ChessPrint();
				chess.Clear();
				AItest_Turn = 3 - AItest_Turn;
				ChessPrint();
			}
			if (chess.hasWin == -1)
			{
				hdc = GetDC(CurrentHwnd());
				std::string sz = "";
				sz += "AI1获胜次数：";
				sz += std::to_string(AIWin[1]);
				sz += "平均每步时长：";
				sz += std::to_string(time[1] * 1000 / num[1]);
				sz += "最长耗费时间：";
				sz += std::to_string(longest[1]);
				TextOut(hdc, 800, 600, TEXT(sz.c_str()), sz.size());

				sz = " ";
				sz += "AI2获胜次数：";
				sz += std::to_string(AIWin[2]);
				sz += "平均每步时长：";
				sz += std::to_string(time[2] * 1000 / num[2]);
				sz += "最长耗费时间：";
				sz += std::to_string(longest[2]);
				TextOut(hdc, 800, 630, TEXT(sz.c_str()), sz.size());

				longest[1] = 0;
				longest[2] = 0;
				ReleaseDC(CurrentHwnd(), hdc);
				chess.Clear();
				AItest_Turn = 3 - AItest_Turn;
			}
			break;
		}

	} while (1);
}