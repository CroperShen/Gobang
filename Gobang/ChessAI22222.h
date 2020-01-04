//===================================================================
//ChessAI2.h
//Created by CroperShen
//last modified at Dec.24.2018
//以“棋子串”为核心的五子棋ai，主要思想是以“串”为核心，计算棋盘价值
//“串”即在四个方向上连续切颜色相同的n个棋子，在这里，为了减少数据量n=0和1时将不会生成串；
//“串”根据串的长度以及两端是否有开口计算串的价值。特别的，当串被阻挡导致延伸后最大长度不能赢得胜利时，其价值总是为0
//棋盘价值即为己方串价值之和-敌方串价值之和。
//网格的价值即为在此网格落子后，导致的棋盘价值变化。
//To do：使用alpha-beta剪枝法进行深度搜索
//=========================================================================
#define DEBUGMODE 1;

#pragma once
#include <stack>
#include "GameRun.h"
#include "cprlib.h"
#include "linkedlist.h"
#include "ChessAI.h"



namespace AIMap2
{
	const int WinNum = 5; 
	const int LinkScore[6] = { 0,0,10,21,63,10000 };  
	const int minIgnoreLength = 4;        //最小忽略长度：当落子的位置距离最近的子大于等于这个长度时，认为此子将不能影响局势，价值取所有落子中的最小值。
	//------------------------调试用------------------------------
#if DEBUGMODE==1
	int SearchNum;       //进行搜索过的节点数;
	int CutInfo[10000];  //剪枝信息，记录剪枝的深度
	int CutNum;          //剪枝数量
	int SumCutOrder;	 //发生剪枝前搜索过多少节点，可评判剪枝效率
	typedef std::string stdstring;
#endif
//------------------------------------------------------------

	class AIMap;
	class String;   //这里的String是串。。不是字符串，是棋子串

	class String                         //棋子串类,作为不含指针的数据类型，可以直接进行浅复制。
	{
	public:
		int d;                           //方向 0为X+Y- 1为X+ 2为X+Y+ 3为Y- 此方向设定保证了在进行先x后y遍历时，负方向的点总是被先遍历
		int length;                      //串长度
		ptype type;                      //串类型，黑子串还是白子串
		int open1, open2;                //负方向开口长度和正方向开口长度
		int startX, startY;              //串头的XY坐标

		String();
		String(String& str0);
		inline int maxlength();
		inline int OpenNum();
		inline int value();

	};
	class AIMap
	{
	public:
		ChessGame game;                  //现在的局势，所有判断都是根据这个局势而下的
		int **value;                     //计算完成后各个位置的价值，根据迭代方法的不同，只有进行计算后才有意义；
		String ****str;                  //每个位置对应的串，是一个串指针的三维数组，str[x][y][d]
		linkedlist<String> strlist;     //存储串的链表，便于遍历所有串

		AIMap(const ChessGame&);             
		AIMap(const AIMap& map0);
		~AIMap();

		bool PointLegal(int x, int y);                  //位置是否合法
		bool Same(int x, int y, ptype t);               //某点是否与另一点相同
		bool IsOpnt(int x, int y, ptype t);             //对某点来说，另一点是否“不可穿越”，包括敌方棋子或边框
		bool CanWin(const ChessGame &g, int x, int y, ptype t);
		int BasePointValue(int x, int y, ptype t);      //计算某个点的基础分数

		int DeepPointValue(ChessGame game,int x,int y,ptype t,int depth,int OpntMaxValue,int SumValue);      //计算某个点的基础分数
		int Put(int x, int y, ptype t);								//在某点放置棋子后棋盘价值的变化

		int CalcBoardValue_func(int x, int y);						//计算棋盘价值的子函数
		int CalcBoardValue(ptype);									//计算棋盘价值
		int CalcBaseValue(ptype);									//计算棋盘价值
		int CalcDeepValue(ptype t,int depth);							//进行深度为n的深度搜索，计算棋盘价值。

	};

	int NewX(int,int,int);
	int NewY(int,int,int);
	bool** SetIgnoreNet(const ChessGame& game,int=minIgnoreLength);
	
	Chsopt ChessAIMain(ChessGame&);

	inline String::String()
	{
	}

	inline String::String(String & str0)
	{
		this->d = str0.d;
		this->length = str0.length;
		this->type = str0.type;
		this->open1 = str0.open1;
		this->open2 = str0.open2;
		this->startX = str0.startX;
		this->startY = str0.startY;
	}

	inline int AIMap2::String::maxlength()
	{
		return open1 + length + open2;
	}

	inline int String::OpenNum()
	{
		return (open1 > 0 ? 1 : 0) + (open2 > 0 ? 1 : 0);
	}

	inline int String::value()
	{
		if (maxlength() < WinNum) return 0;
		return LinkScore[min(WinNum, length)] * OpenNum();
	}

	AIMap::AIMap(const ChessGame & game0) :game(game0)
	{
		int i, j, k;
		value = new int*[game.cx];
		for (i = 0; i < game.cx; i++) value[i] = new int[game.cy];
		str = new String***[game.cx];
		for (i = 0; i < game.cx; i++)
		{
			str[i] = new String**[game.cy];
			for (j = 0; j < game.cy; j++)
			{
				str[i][j] = new String*[4];
				for (k = 0; k < 4; k++) str[i][j][k] = NULL;
			}
		}
	}

	inline AIMap::AIMap(const AIMap & map0):AIMap(map0.game)
	{
		int i, j, x, y;
		String *str1;
		strlist = map0.strlist;
		for (i = 0; i < strlist.size(); i++)
		{
			str1 = &strlist[i];
			for (j = 0; j < strlist[i].length; j++)
			{
				x = NewX(str1->startX, j, str1->d);
				y = NewY(str1->startY, j, str1->d);
				str[x][y][str1->d] = str1;
			}
		}
	}

	AIMap::~AIMap()
	{
		for (int i = 0; i < game.cx; i++)
		{
			for (int j = 0; j < game.cy; j++)
			{
				delete[]str[i][j];
			}
			delete[] str[i];
			delete[] value[i];
		}
		delete[] str;
		delete[] value;
	}


	inline bool AIMap::PointLegal(int x, int y)
	{
		return (x >= 0) && (x < game.cx) && (y >= 0) && (y < game.cy);
	}

	inline bool AIMap::Same(int x, int y, ptype t)
	{
		if ((x < 0) || (x >= game.cx) || (y < 0) || (y >= game.cy)) return false;
		return (game.cell[x][y] == t);
	}


	inline bool AIMap::IsOpnt(int x, int y, ptype t)
	{
		t = (ptype)(3 - (int)t);
		if ((x < 0) || (x >= game.cx) || (y < 0) || (y >= game.cy)) return true;
		return (game.cell[x][y] == t);
	}

	inline bool AIMap::CanWin(const ChessGame & g, int x, int y, ptype t)
	{
		int i, j;
		for (int d = 0; d < 3; d++)
		{
			i = 1;
			while (Same(NewX(x, 0 - i, d), NewY(y, 0 - i, d), t))
			{
				i++;
				if (i >= 5) return true;
			}
			j = 1;
			while (Same(NewX(x, j, d), NewY(y, j, d), t))
			{
				j++;
				if (i+j-1>= 5) return true;
			}
		}
		return false;
	}

	inline int AIMap::BasePointValue(int x, int y, ptype t)
	{
		AIMap map = *this;
		return map.Put(x,y,t);
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
			return x + i;
		case 3:
			return x;
		case 4:
			return x - i;
		case 5:
			return x - i;
		case 6:
			return x - i;
		case 7:
			return x;
		}
		return 0;
	}

	inline int NewY(int y, int i, int d)
	{
		switch (d)
		{
		case 0:
			return y - i;
		case 1:
			return y;
		case 2:
			return y + i;
		case 3:
			return y + i;
		case 4:
			return y + i;
		case 5:
			return y;
		case 6:
			return y - i;
		case 7:
			return y - i;
		}
		return 0;
	}

	bool** SetIgnoreNet(const ChessGame & game, int l)
	{
		bool** a;
		int x0, y0;
		x0 = 0x80000000;
		y0 = 0x80000000;
		a = new bool*[game.cx];
		for (int i = 0; i < game.cx; i++) a[i] = new bool[game.cy];
		for (int i = 0; i < game.cx; i++) for (int j = 0; j < game.cy; j++) a[i][j] = true;
		for (int i = 0; i < game.cx; i++) for (int j = 0; j < game.cy; j++)
		{
			if (game.cell[i][j] == t_null) continue;
			for (int i2 = max(max(x0+l,i-l+1),1); i2 < min(i + l,game.cx); ++i2)
				for (int j2 = max(0,j - l + 1);j2 < min(game.cy,j + l); ++j2)
					a[i2][j2] = false;

			if (j > y0)
			{
				for (int i2 = max(0,i - l + 1); i2 <min(x0 + l,game.cx); ++i2)
					for (int j2 = max(0,max(y0, j - l + 1)); j2 < min(game.cy,j + l); ++j2)
						a[i2][j2] = false;
			}
			else
			{
				for (int i2 = max(0, i - l + 1); i2 < min(x0 + l, game.cx); ++i2)
					for (int j2 =max(0,j - l +1); j2 < min(game.cy,min(j + l,y0-l+1)); ++j2)
						a[i2][j2] = false;
			}
			x0 = i;
			y0 = j;
		}
		return a;
	}


	inline int AIMap::DeepPointValue(ChessGame game,int x, int y, ptype t, int depth, int OpntMinValue, int value0)
	{
		AIMap *map;
		bool** ignet;
		int minValue, value;
		int i, j;
		if (CanWin(game, x, y, t)) return LinkScore[WinNum];          //如果能直接胜利，返回一个很大的分数;

#if DEBUGMODE==1//------------------------------------------------
		int CutOrder = 0;
		++SearchNum;
#endif//------------------------------------------------------

		game.cell[x][y] =t;                                           //否则在此点下棋
		if (depth == 0)                                               //如果深度为0，则计算棋盘价值，落子价值=现价值-初价值
		{
			map = new AIMap(game);
			value0 = map->CalcBoardValue(t) - value0;
			delete map;
			return value0;
		}
		else                                                                             //如果深度不为0
		{
			ignet = SetIgnoreNet(game, minIgnoreLength-1);							
			minValue = 0x7FFFFFFF;									//maxValue初始值为无穷大
			t = (ptype)(3 - (int)t);

			
			Chsopt opt =AIMap1::ChessAIMain(game,t);
			
			i = opt.x;
			j = opt.y;
			value = DeepPointValue(game, i, j, t, depth - 1, 0 - minValue, 0 - value0);                
			minValue = 0 - value;
			ignet[i][j] = true;
#if DEBUGMODE==1//----------------------------------------------
			if (minValue <= OpntMinValue)
			{ 
				++CutInfo[depth];
				++CutNum;
				SumCutOrder += CutOrder;
			}
#endif//--------------------------------------------------------------
			if (minValue > OpntMinValue)
			{ 
				for (i = 0; i < game.cx; i++) for (j = 0; j < game.cy; j++)
				{
					if (ignet[i][j]) continue;
					if (game.cell[i][j] != t_null) continue;
#if DEBUGMODE==1//---------------------------------------
					++CutOrder;
#endif			//----------------------------------------------
					value =DeepPointValue(game, i, j, t, depth - 1,0-minValue, 0-value0);                //获得在某个子节点的价值，对对手来说，所有价值都取负
					value = 0 - value;                                                                   
					if (minValue > value) minValue = value;           //因为对方总是会对选择最大的价值                
					if (minValue <= OpntMinValue)			          //但如果minValue小于上一层的最小值，那么己方则不可能选择这个分支，进行剪枝；
					{
#if DEBUGMODE==1//------------------------------------------------------
						++CutInfo[depth] ;
						++CutNum;
						SumCutOrder += CutOrder;
#endif//----------------------------------------------------------------
						break;
					}
				}
			}
			for (i = 0; i < game.cx; i++) delete[]ignet[i];
			delete[]ignet;
		}

		return minValue;
	}

	inline int AIMap::Put(int x, int y, ptype t)             //计算当在位置x，y放下棋子时价值的变化
	{
		int x1, y1;
		int i, j, x2, y2;
		String *str1, *str2;
		int value0 = 0;                                         //value0参与计算的初始价值
		int value1 = 0;                                         //value1参与计算的变化后价值
		if (game.cell[x][y] != t_null) return -1;                //当选择点有子时，没有必要进行计算
		for (int d = 0; d <= 3; d++)
		{
			str1 = NULL;                                            //先向负方向遍历
			x1 = NewX(x, -1, d);
			y1 = NewY(y, -1, d);
			if (PointLegal(x1, y1))                                //只有当新的坐标合法，才继续计算
			{
				str1 = str[x1][y1][d];                             //str1为获得的新串
				if (str1 != NULL)                                   //当新串存在，可能的情况
				{
					if (str1->type != t)
					{
						value0 -= str1->value();
						str1->open2 = 0;
						value1 -= str1->value();
					}
					else
					{
						value0 += str1->value();
						str1->length += 1;
						str1->open2 -= 1;
						value1 += str1->value();
						str[x][y][d] = str1;

					}
				}
				else
				{
					if (game.cell[x1][y1] == t)
					{
						str1 = &strlist.Append();
						str1->d = d;
						str1->startX = x1;
						str1->startY = y1;
						str1->length = 2;
						//str1->map = this;
						str1->type = t;
						j = 2;
						while (!IsOpnt(NewX(x, 0 - j, d), NewY(y, 0 - j, d), t)) j++;
						str1->open1 = j - 2;
						j = 1;
						while (!IsOpnt(NewX(x, j, d), NewY(y, j, d), t)) j++;
						str1->open2 = j - 1;

						str[x1][y1][d] = str1;
						str[x][y][d] = str1;
						value1 += str1->value();
					}
					else
					{
						for (i = 2; i <= WinNum - 2; i++)
						{
							x1 = NewX(x, 0 - i, d);
							y1 = NewY(y, 0 - i, d);
							if (!PointLegal(x1, y1)) break;
							str1 = str[x1][y1][d];
							if (str1 != NULL)
							{
								if (str1->type != t)
								{
									value0 -= str1->value();
									str1->open2 = i - 1;
									value1 -= str1->value();
									str1 = NULL;
									
								}
								break;
							}
						}
					}
				}
			}
			str2 = NULL;
			x1 = NewX(x, 1, d);
			y1 = NewY(y, 1, d);
			if (PointLegal(x1, y1))
			{
				str2 = str[x1][y1][d];
				if (str2 != NULL)
				{
					if (str2->type == t)
					{
						if (str1 != NULL)
						{
							value0 += str1->value();
							value0 += str2->value();
							str1->length = str1->length + str2->length;
							str1->open2 = str2->open2;
							for (j = 1; j <= str2->length; j++) str[NewX(x, j, d)][NewY(y, j, d)][d] = str1;
							strlist.Delete(str2);
							value1 += str1->value();
						}
						else
						{
							value0 += str2->value();
							str2->startX = x;
							str2->startY = y;
							str2->length += 1;
							str2->open1 -= 1;
							str[x][y][d] = str2;
							value1 += str2->value();
						}
					}
					else
					{
						value0 -= str2->value();
						str2->open1 = 0;
						value1 -= str2->value();
					}
				}
				else
				{
					if (game.cell[x1][y1] == t)
					{
						if (str1 != NULL)
						{
							value0 += str1->value();
							str1->length += 1;
							str1->open2 -= 1;
							str[x1][y1][d] = str1;
							value1 += str1->value();
						}
						else
						{
							str2 = &strlist.Append();
							str2->d = d;
							str2->startX = x;
							str2->startY = y;
							str2->length = 2;
							//str2->map = this;
							str2->type = t;
							j = 1;
							while (!IsOpnt(NewX(x, 0 - j, d), NewY(y, 0 - j, d), t)) j++;
							str2->open1 = j - 1;
							j = 2;
							while (!IsOpnt(NewX(x, j, d), NewY(y, j, d), t)) j++;
							str2->open2 = j - 2;

							str[x1][y1][d] = str2;
							str[x][y][d] = str2;
							value1 += str2->value();
						}
					}
					else
					{
						for (i = 2; i <= WinNum - 2; i++)
						{
							x1 = NewX(x, i, d);
							y1 = NewY(y, i, d);
							if (!PointLegal(x1, y1)) break;
							str2 = str[x1][y1][d];
							if (str2 != NULL)
							{
								if (str2->type != t)
								{
									value0 -= str2->length;
									str2->open2 = i - 1;
									value1 -= str2->length;
									str2 = NULL;
									
								}
								break;
							}
						}
					}
				}
			}
		}
		return value1 - value0;
	}

	inline int AIMap::CalcBoardValue_func(int x, int y)
	{
		int i, k, i2, j2, x1, y1;
		ptype t = game.cell[x][y];

		if (t == t_null) return 0;

		for (int d = 0; d <= 3; d++)
		{
			if (str[x][y][d] == NULL)
			{
				i = 1;
				x1 = NewX(x, i, d);
				y1 = NewY(y, i, d);
				while (Same(x1, y1, t))  //遍历+X方向，计算同色棋子的数目 
				{
					if (str[x][y][d] == NULL)
					{
						str[x][y][d] = &(strlist.Append());
						str[x][y][d]->d = d;
						str[x][y][d]->startX = x;
						str[x][y][d]->startY = y;
						str[x][y][d]->length = 1;
						str[x][y][d]->type = t;
					};
					str[x][y][d]->length = i+1;
					str[x1][y1][d] = str[x][y][d];
					i++;
					x1 = NewX(x, i, d);
					y1 = NewY(y, i, d);
				}
				if (str[x][y][d]!=NULL)
				{ 
					i2 = i;
					while (!IsOpnt(NewX(x, i2, d), NewY(y, i2, d), t)) i2++;             //继续遍历-方向，到对手的子或边界以外停止
					j2 = -1;
					while (!IsOpnt(NewX(x, j2, d), NewY(y, j2, d), t)) j2--;            //继续遍历+方向，到对手的子或边界以外停止

					str[x][y][d]->open1 = -1-j2;
					str[x][y][d]->open2 = i2 - i;
				}
			}
		}
		return 0;
	}


	inline int AIMap::CalcBoardValue(ptype t)
	{
		int l;
		int sumValue;
		AIMap *map = this;
		for (int i = 0; i < game.cx; i++) for (int j = 0; j < game.cy; j++) CalcBoardValue_func(i, j);
		l = strlist.size();
		sumValue = 0;
		for (int i = 0; i < l; i++)
		{
			if (strlist[i].type==t)
			{
		    	sumValue += strlist[i].value();
			}
			else
			{
				sumValue -= strlist[i].value();
			}
		}
		return sumValue;
	}

	inline int AIMap::CalcBaseValue(ptype t)
	{
		int n;
		CalcBoardValue(t);
		for (int i = 0; i < game.cx; i++) for (int j = 0; j < game.cy; j++)
		{
			if ((i == 1) && (j == 13))
			{
				i = i;
			}
			n = min(min(i, game.cx - i - 1), min(j, game.cy - j - 1));
			n = min(9, n);
			value[i][j] = BasePointValue(i, j, t) * 10 + n;
		}
		return 0;
	}




	inline int AIMap::CalcDeepValue(ptype t, int depth)
	{
		int value0 = CalcBoardValue(t);
		int minvalue = 0;
		int n=0x80000000;
		bool** ignet = SetIgnoreNet(game);

		//-----------------------调试用-----------------------
#if DEBUGMODE==1
		SearchNum = 0;
		CutNum = 0;
		SumCutOrder = 0;
		for (int i = 0; i <= depth; i++) CutInfo[i] = 0;
#endif
		//----------------------------------------------------
		for (int i = 0; i < game.cx; i++) for (int j = 0; j < game.cy; j++)
		{
			if (ignet[i][j]) continue;
			if (game.cell[i][j] != t_null)
			{
				value[i][j] = -1000000;
				continue;
			}
			value[i][j] = DeepPointValue(game, i, j, t, depth, 0x80000000, value0);
			if (value[i][j] < minvalue) minvalue = value[i][j];
		}

		for (int i = 0; i < game.cx; i++) for (int j = 0; j < game.cy; j++)
		{
			if (ignet[i][j]) value[i][j] = minvalue;
			n = min(min(i, game.cx - i - 1), min(j, game.cy - j - 1));
			n = min(9, n);
			value[i][j] = 10 * value[i][j] + n;
		}
		
#if DEBUGMODE==1 //----------------------------调试用--------------------------
		HDC hdc = GetDC(CurrentHwnd());
		int xText = 100;
		int yText = 700;
		float AveCutOrder;
		stdstring sz;
		sz += "总共搜索节点：";
		sz += std::to_string(SearchNum);
		sz += "   剪枝数量：";
		sz+= std::to_string(CutNum);
		TextOut(hdc, xText, yText, TEXT(sz.c_str()), sz.size());

		xText = 100;
		yText = 720;
		sz = "";
		sz += "平均剪枝次序";
		if (CutNum>0) 
		{ 
			AveCutOrder = (float)SumCutOrder / CutNum;
			sz += std::to_string(AveCutOrder);
		}
		else
		{
			sz += "NA";
		}
		sz += "相对剪枝率";
		sz += std::to_string((float)CutNum/SearchNum);
		TextOut(hdc, xText, yText, TEXT(sz.c_str()), sz.size());

	
#endif			//--------------------------------------------------------

		for (int i = 0; i < game.cx; i++) delete ignet[i];
		delete ignet;
		return 0;
	}


	void DisplayBoardValue()
	{
		AIMap map(CurrentGame().game);
		HDC hdc;
		bool **b;
		std::string sz;
		map.CalcDeepValue(t_black,3);
		
		hdc = GetDC(CurrentHwnd());
		
		for (int i = 0; i < map.game.cx; i++) for (int j = 0; j < map.game.cy; j++)
		{

			sz = std::to_string(map.value[i][j] / 10);
			if (map.value[i][j] < -10000) sz = "-";

			TextOut(hdc, 700 + 33 * i, 100 + 33 * j, "       ", 7);
			TextOut(hdc, 700 + 33 * i, 100 + 33 * j, TEXT(sz.c_str()), sz.size());
		}
		ReleaseDC(CurrentHwnd(), hdc);
	}

	void DisplayBoardValue2()
	{
		AIMap map(CurrentGame().game);
		HDC hdc;
		bool **b = SetIgnoreNet(CurrentGame().game, 4);
		std::string sz;
		hdc = GetDC(CurrentHwnd());

		for (int i = 0; i < map.game.cx; i++) for (int j = 0; j < map.game.cy; j++)
		{

			sz = (b[i][j]) ? "true" : "false";

			TextOut(hdc, 700 + 40 * i, 100 + 40 * j, "       ", 7);
			TextOut(hdc, 700 + 40 * i, 100 + 40 * j, TEXT(sz.c_str()), sz.size());
		}
		ReleaseDC(CurrentHwnd(), hdc);
		for (int i = 0; i < map.game.cx; i++) delete[] b[i];
		delete[]b;
	}


	Chsopt ChessAIMain(ChessGame& g, ptype t)
	{
		std::stack<Chsopt> optstk;
		AIMap map(g);
		int MaxScore = 0x80000000;
		Chsopt opt;
		bool b = false;
		opt.id = 1;

		map.CalcDeepValue(t,3);

		for (int i = 0; i < g.cx; i++) for (int j = 0; j < g.cy; j++)
		{
			if (map.value[i][j] > MaxScore)
			{
				MaxScore = map.value[i][j];
				ClearStack(optstk);
			};
			if (map.value[i][j] == MaxScore)
			{
				opt.x = i;
				opt.y = j;
				optstk.push(opt);
			}
		}
		return StackPickRandom(optstk);
	}

}
void BoardValueMessage(HWND hwnd)
{
	AIMap2::AIMap map = AIMap2::AIMap(CurrentGame().game);
	int i = map.CalcBoardValue(t_black);
	std::string sz = "棋盘价值：";
	sz += (std::to_string(i));
	MessageBox(hwnd, TEXT(sz.c_str()), " ", 0);
}