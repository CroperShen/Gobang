#pragma once
#include <Windows.h>
enum ptype{t_null=0,t_black=1,t_white=2};   //棋子类型 
int DefLen = 15;

struct ChessBoard;
class ChessGame;
class Chess;

class ChessBoardHeight
{
friend struct ChessBoard;
private:
	ChessBoard*p;
public:
	operator int();
	ChessBoardHeight& operator=(int);
};

struct ChessBoard                  //棋盘结构
{
	int top;
	int left;
	int width;
	int framewidth;
	int cx;
	int cy;
	ChessBoardHeight height;

	ChessBoard();
	void print(HDC hdc);
	void print(HDC hdc,ChessGame&);
	void print(HDC hdc,ChessGame&,int,int);

	int GetPixLineX(int);
	int GetPixLineY(int);
};

class ChessGame
{
public:
	const int cx;
	const int cy;
	int pnum;
	ptype **cell;

	ChessGame();
	ChessGame(int x, int y);
	ChessGame(const ChessGame&);
	~ChessGame();

	ChessGame& operator=(const ChessGame&);
	bool operator==(const ChessGame&)const;
	bool operator!=(const ChessGame&)const;

};

class Chess
{
	friend Chess& CurrentGame();
private:
	Chess();
	~Chess();
public:
	ChessGame game;
	ChessBoard board;
	int WinNum;        //胜利需要连成的子的数目，默认是5
	int hasWin;        //是否已经判定胜利，胜利之后会进入锁定状态
	int Turn;          //该黑棋动为1，该白旗动为2

	

	void print(HDC hdc);
	void print(HDC hdc,int,int);
	ptype& pices(int, int);
	void Put(int,int,ptype);             //放置棋子
	bool PutBlack(int, int);
	bool PutWhite(int, int);
	bool PutBlackP(int, int);
	bool PutWhiteP(int, int);
	void Clear();

private:int judge_func(int,int);
public:int judge();                      //判断胜负,0为无胜负，1为黑胜，2为白胜
};

ChessBoard::ChessBoard()
{
	top = 100;
	left = 100;
	width = 560;
	cx = DefLen;
	cy = DefLen;
	framewidth = 25;

	height.p = this;
}

void ChessBoard::print(HDC hdc)
{
	int height = width * (cy - 1) / (cx - 1);
	int bottom = top + height;
	int right = left + width;
	int z;

	Rectangle(hdc, left - framewidth, top - framewidth, right + framewidth+1, bottom + framewidth+1);
	Rectangle(hdc, left, top, right+1, bottom+1);
	for (int i = 1; i < cx - 1; i++)
	{
		z = left + width * i / (cx - 1);
		MoveToEx(hdc, z, top, NULL);
		LineTo(hdc, z, bottom);
	}

	for (int i = 1; i < cy - 1; i++)
	{
		z = top + height * i / (cy - 1);
		MoveToEx(hdc, left, z, NULL);
		LineTo(hdc, right, z);
	}
}

void ChessBoard::print(HDC hdc,ChessGame& game)
{
	int mX = min(game.cx, cx);
	int mY = min(game.cy, cy);
	int r = (int)(0.8*width / (cx - 1) / 2);

	int x;
	int y;

	HBRUSH hBrush, hPreBrush;

	print(hdc);
	for (int j = 0; j < mY; j++) for (int i = 0; i < mX; i++)
	{
		x = left + i * width / (cx - 1);
		y = top + j * height / (cy - 1);
		hBrush = 0;
		if (game.cell[i][j] == t_black) hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
		if (game.cell[i][j] == t_white) hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		if (game.cell[i][j] != t_null)
		{
			hPreBrush = (HBRUSH)SelectObject(hdc, hBrush);
			Ellipse(hdc, x - r, y - r, x + r, y + r);
			SelectObject(hdc, hPreBrush);
		}
	}
}

void ChessBoard::print(HDC hdc,ChessGame& game,int i,int j)
{
	int r = (int)(0.8*width / (cx - 1) / 2);
	int x = left + i * width / (cx - 1);
	int y = top + j * height / (cy - 1);
	int x0, x1, y0, y1;
	HBRUSH hBrush, hPreBrush;
	HPEN hPen, hPrePen;
	if (game.cell[i][j] == t_null)
	{
		hPen = (HPEN)GetStockObject(WHITE_PEN);
		hPrePen = (HPEN)SelectObject(hdc, hPen);
		Ellipse(hdc, x - r, y - r, x + r, y + r);
		SelectObject(hdc, hPrePen);
		x0 = x - r;
		x1 = x + r;
		y0 = y - r;
		y1 = y + r;
		if (i == 0) x0 = x;
		if (i == game.cx - 1) x1 = x;
		if (j == 0) y0 = y;
		if (j == game.cy - 1) y1 = y;
		MoveToEx(hdc, x0, y, NULL);
		LineTo(hdc, x1, y);
		MoveToEx(hdc, x, y0,NULL);
		LineTo(hdc, x, y1);
	}
	else
    {
		hBrush = 0;
	    if (game.cell[i][j] == t_black) hBrush = (HBRUSH)GetStockObject(BLACK_BRUSH);
	    if (game.cell[i][j] == t_white) hBrush = (HBRUSH)GetStockObject(WHITE_BRUSH);
		hPreBrush = (HBRUSH)SelectObject(hdc, hBrush);
		Ellipse(hdc, x - r, y - r, x + r, y + r);
		SelectObject(hdc, hPreBrush);
	}
}

inline int ChessBoard::GetPixLineX(int x)
{
	if (x < left) return 0 - (((cx - 1) * (left - x) + width / 2) / width);
	return ((cx - 1) * (x - left) + width / 2) / width;
}

inline int ChessBoard::GetPixLineY(int y)
{
	int height = width * (cy - 1) / (cx - 1);
	if (y < top) return 0 - (((cy - 1) * (top - cy) + height / 2) / height);
	return ((cy - 1) * (y - top) + height / 2) / height;
}



ChessGame::ChessGame() :cx(DefLen), cy(DefLen)
{
	cell = new ptype*[cx];
	for (int i = 0; i < cx; i++) cell[i] = new ptype[cy];
	for (int j = 0; j < cy; j++) for (int i = 0; i < cx; i++) cell[i][j] = t_null;
};
ChessGame::ChessGame(int x, int y) :cx(x), cy(y)
{
	cell = new ptype*[cx];
	for (int i = 0; i < cx; i++) cell[i] = new ptype[cy];
	for (int j = 0; j < cy; j++) for (int i = 0; i < cx; i++) cell[i][j] = t_null;
}
inline ChessGame::ChessGame(const ChessGame & game0) :ChessGame(game0.cx,game0.cy)
{
	for (int i = 0; i < cx; i++) for (int j = 0; j < cy; j++) cell[i][j] = game0.cell[i][j];
}
;

ChessGame::~ChessGame()
{
	static int n = 0;
	n++;
	for (int i = 0; i < cx; i++) delete[](cell[i]);
	delete[] cell;
}

inline ChessGame & ChessGame::operator=(const ChessGame & game0)
{
	for (int i = 0; i < cx; i++) for (int j = 0; j < cy; j++) cell[i][j] = game0.cell[i][j];
	return *this;
}

inline bool ChessGame::operator==(const ChessGame & game0) const
{
	if ((cx != game0.cx) || (cy != game0.cy)) return false;
	for (int i = 0; i < cx; i++) for (int j = 0; j < cy; j++)
	{
		if (cell[i][j] != game0.cell[i][j]) return false;
	}
	return true;
}

inline bool ChessGame::operator!=(const ChessGame & game0) const
{
	return (!operator==(game0));
}

inline Chess::Chess()
{
	WinNum = 5;
}

inline Chess::~Chess()
{
}

void Chess::print(HDC hdc)
{
	board.print(hdc,game);
}

inline void Chess::print(HDC hdc,int i, int j)
{
	board.print(hdc,game, i, j);
}

inline ptype& Chess::pices(int x, int y)
{
	if ((x >= game.cx) || (y >= game.cy) || (x < 0) || (y < 0)) throw "param overflow";
	return game.cell[x][y];
}

inline void Chess::Put(int x, int y, ptype t)
{
	pices(x, y) = t;
}

inline bool Chess::PutBlack(int x, int y)
{
	if (pices(x, y) == t_null)
	{
		pices(x, y) = t_black;
		game.pnum++;
		Turn = 2;
		return true;
	}
	return false;
}

inline bool Chess::PutWhite(int x, int y)
{
	if (pices(x, y) == t_null)
	{
		pices(x, y) = t_white;
		game.pnum++;
		Turn = 1;
		return true;
	}
	return false;
}

inline bool Chess::PutBlackP(int xPix, int yPix)
{
	int x = board.GetPixLineX(xPix);
	int y = board.GetPixLineY(yPix);
	if ((x < 0) || (x >= game.cx) || (y < 0) || (y >= game.cy)) return false;
	if (pices(x, y) != t_null) return false;
	pices(x, y) = t_black;
	game.pnum++;
	return true;
}

inline bool Chess::PutWhiteP(int xPix, int yPix)
{
	int x = board.GetPixLineX(xPix);
	int y = board.GetPixLineY(yPix);
	if ((x < 0) || (x >= game.cx) || (y < 0) || (y >= game.cy)) return false;
	if (pices(x, y) != t_null) return false;
	pices(x, y) = t_white;
	game.pnum++;
	return true;
}

inline void Chess::Clear()
{
	for (int j = 0; j < game.cy; j++) for (int i = 0; i < game.cx; i++) pices(i, j) = t_null;
	hasWin = 0;
	game.pnum = 0;
	Turn = 1;
}

inline int Chess::judge_func(int x, int y)  //判断某一个子是否与其后面的子导致胜利
{
	int i, t;
	if (pices(x, y) == t_null) return 0;
	t = pices(x, y);
	if (game.cx - x >= WinNum) for (i = 1; i < WinNum; i++)
	{
		if (pices(x + i, y) != t) break;
		if (i + 1 == WinNum) return t;
	}
	if (game.cy - y >= WinNum) for (i = 1; i < WinNum; i++)
	{
		if (pices(x, y + i) != t) break;
		if (i + 1 == WinNum) return t;
	}
	if ((game.cy - y >= WinNum) && (game.cx - x >= WinNum)) for (i = 1; i < WinNum; i++)
	{
		if (pices(x + i, y + i) != t) break;
		if (i + 1 == WinNum) return t;
	}
	if ((x >= WinNum - 1) && (game.cy - y >= WinNum)) for (i = 1; i < WinNum; i++)
	{
		if (pices(x - i, y + i) != t) break;
		if (i + 1 == WinNum) return t;
	}
	return 0;
}

inline int Chess::judge()
{
	int i, j;
	int t;
	for (j = 0; j < game.cy; j++) for (i = 0; i < game.cx; i++)
	{
		t = judge_func(i, j);
		if (t != 0)
		{
			hasWin = t;
			break;
		}
	}
	if (hasWin == 0)
		if (game.pnum >= game.cx*game.cy) hasWin = -1;
	return hasWin;
}
ChessBoardHeight::operator int()
{
	return (p->cy - 1)*p->width / (p->cx - 1);
}

ChessBoardHeight & ChessBoardHeight::operator=(int i)
{
	i = i * (p->cy - 1) / (p->cx - 1);
	p->width = i;
	return *this;
}
