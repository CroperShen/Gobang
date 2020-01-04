#pragma once
#include <stack>
#include "GameRun.h"
#include "cprlib.h"

namespace AIMap1
{
	class AIMap;

	class AIMap
	{
	public:
		ChessGame const *game;
		int **bValue;
		int **wValue;

		AIMap(const ChessGame&);
		~AIMap();

		bool Same(int x, int y, ptype t);
		bool IsOpnt(int x, int y, ptype t);
		int PointValue(int x, int y, ptype t);

		void CalcValue(ptype);
	};


	Chsopt ChessAIMain(ChessGame&);


	AIMap::AIMap(const ChessGame & game0) :game(&game0)
	{
		int i;
		bValue = new int*[game->cx];
		wValue = new int*[game->cx];
		for (i = 0; i < game->cx; i++) bValue[i] = new int[game->cy];
		for (i = 0; i < game->cx; i++) wValue[i] = new int[game->cy];
	}

	AIMap::~AIMap()
	{
		for (int i = 0; i < game->cx; i++) delete[]bValue[i];
		for (int i = 0; i < game->cx; i++) delete[]wValue[i];
		delete[] bValue;
		delete[] wValue;
	}


	inline bool AIMap::Same(int x, int y, ptype t)
	{
		if ((x < 0) || (x >= game->cx) || (y < 0) || (y >= game->cy)) return false;
		return (game->cell[x][y] == t);
	}


	inline bool AIMap::IsOpnt(int x, int y, ptype t)
	{
		t = (ptype)(3 - (int)t);
		if ((x < 0) || (x >= game->cx) || (y < 0) || (y >= game->cy)) return true;
		return (game->cell[x][y] == t);
	}

	inline int NewX(int x, int i, int d)
	{
		switch (d)
		{
		case 0:
			return x + i;
		case 1:
			return x + i;
		case 2:
			return x;
		case 3:
			return x - i;
		case 4:
			return x - i;
		case 5:
			return x - i;
		case 6:
			return x;
		case 7:
			return x + i;
		}
		return 0;
	}

	inline int NewY(int y, int i, int d)
	{
		switch (d)
		{
		case 0:
			return y;
		case 1:
			return y + i;
		case 2:
			return y + i;
		case 3:
			return y + i;
		case 4:
			return y;
		case 5:
			return y - i;
		case 6:
			return y - i;
		case 7:
			return y - i;
		}
		return 0;
	}

	inline int AIMap::PointValue(int x, int y, ptype t)     //计算某个点的分数
	{
		static int WinNum = 5;
		int Sum;
		int *LinkScore;
		int i, j, k;
		int i2, j2;
		if (game->cell[x][y] != t_null) return 0;     //如果要求的格子已经有子，那么算个毛线啊

		LinkScore = new int[WinNum + 1];
		LinkScore[1] = 0;
		LinkScore[2] = 1000;
		for (int i = 3; i <= WinNum; i++) LinkScore[i] = LinkScore[i - 1] * 2 + 100;
		LinkScore[WinNum] *= 10;

		Sum = min(min(x + 1, game->cx - x), min(y + 1, game->cy - y));   //基础分，离边越近分越低，最高10分,最低1分；
		for (int d = 0; d <= 3; d++)
		{
			i = 1;
			j = 1;
			while (Same(NewX(x, i, d), NewY(y, i, d), t)) i++;              //遍历+X方向，计算同色棋子的数目
			while (Same(NewX(x, j, d + 4), NewY(y, j, d + 4), t)) j++;        //遍历-X方向，计算同色棋子的数目	                                          
			i2 = i;
			j2 = j;
			if (d == 2)
			{
				d = d;
			}
			while (!IsOpnt(NewX(x, i2, d), NewY(y, i2, d), t)) i2++;             //继续遍历X-方向，到对手的子或边界以外停止
			while (!IsOpnt(NewX(x, j2, d + 4), NewY(y, j2, d + 4), t)) j2++;     //继续遍历X+方向，到对手的子或边界以外停止
			if (i2 + j2 - 1 >= WinNum)                                           //如果X方向的空间不足以达到胜利（空间不足5），那么这将毫无意义
			{
				k = min(i + j - 1, WinNum);
				if (k == WinNum)                                     //如果这一步直接胜利，那么不需要计算额外空间的问题
				{
					Sum += LinkScore[k];
				}
				else
				{
					if (i2 > i) Sum += LinkScore[k];                 //如果X-方向有额外空间，那么计算分数
					if (j2 > j) Sum += LinkScore[k];                 //如果X+方向有额外空间，那么计算分数
				}
			}
		}
		return Sum;
	}


	inline void AIMap::CalcValue(ptype t)
	{
		int cx = game->cx;
		int cy = game->cy;
		int** value = NULL;
		int *p;

		if (t == t_black) value = bValue;
		if (t == t_white) value = wValue;

		for (int i = 0; i < cx; i++) for (int j = 0; j < cy; j++)
		{
			p = &(value[i][j]);
			*p = PointValue(i, j, t);
			if (*p <= 0)
			{
				p = p;
			}
		}
	}




	Chsopt ChessAIMain(ChessGame& g, ptype t)
	{
		std::stack<Chsopt> optstk;
		AIMap map(g);
		int OwnMaxScore = -1;
		int OpntMaxScore = -1;
		int **OwnValue = NULL;
		int **OpntValue = NULL;
		Chsopt opt;
		bool b = false;
		opt.id = 1;

		map.CalcValue(t_black);
		map.CalcValue(t_white);
		if (t == t_black)
		{
			OwnValue = map.bValue;
			OpntValue = map.wValue;
		}
		if (t == t_white)
		{
			OwnValue = map.wValue;
			OpntValue = map.bValue;
		}

		for (int i = 0; i < map.game->cx; i++) for (int j = 0; j < map.game->cy; j++)
		{
			if (OwnValue[i][j] > OwnMaxScore)
			{
				OwnMaxScore = OwnValue[i][j];
				if (OwnMaxScore + 10 > OpntMaxScore) ClearStack(optstk);
				if (OwnMaxScore + 10 > OpntMaxScore) b = false;
			}
			if (OpntValue[i][j] > OpntMaxScore)
			{
				OpntMaxScore = OpntValue[i][j];
				if (OwnMaxScore + 10 <= OpntMaxScore) ClearStack(optstk);
				if (OwnMaxScore + 10 <= OpntMaxScore) b = false;
			}
			int test1 = max(OwnValue[i][j] + 10, OpntValue[i][j]);
			int test2 = max(OwnMaxScore + 10, OpntMaxScore);
			if (max(OwnValue[i][j] + 10, OpntValue[i][j]) == max(OwnMaxScore + 10, OpntMaxScore))
			{
				opt.x = i;
				opt.y = j;
				optstk.push(opt);
				b = true;
			}
			if (b = false)
			{
				b = b;
			}
		}

		return StackPickRandom(optstk);
	}
}