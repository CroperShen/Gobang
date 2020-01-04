//===================================================================
//ChessAI2.h
//Created by CroperShen
//last modified at Dec.24.2018
//�ԡ����Ӵ���Ϊ���ĵ�������ai����Ҫ˼�����ԡ�����Ϊ���ģ��������̼�ֵ
//�����������ĸ���������������ɫ��ͬ��n�����ӣ������Ϊ�˼���������n=0��1ʱ���������ɴ���
//���������ݴ��ĳ����Լ������Ƿ��п��ڼ��㴮�ļ�ֵ���ر�ģ��������赲�����������󳤶Ȳ���Ӯ��ʤ��ʱ�����ֵ����Ϊ0
//���̼�ֵ��Ϊ��������ֵ֮��-�з�����ֵ֮�͡�
//����ļ�ֵ��Ϊ�ڴ��������Ӻ󣬵��µ����̼�ֵ�仯��
//To do��ʹ��alpha-beta��֦�������������
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
	const int minIgnoreLength = 4;        //��С���Գ��ȣ������ӵ�λ�þ���������Ӵ��ڵ����������ʱ����Ϊ���ӽ�����Ӱ����ƣ���ֵȡ���������е���Сֵ��
	//------------------------������------------------------------
#if DEBUGMODE==1
	int SearchNum;       //�����������Ľڵ���;
	int CutInfo[10000];  //��֦��Ϣ����¼��֦�����
	int CutNum;          //��֦����
	int SumCutOrder;	 //������֦ǰ���������ٽڵ㣬�����м�֦Ч��
	typedef std::string stdstring;
#endif
//------------------------------------------------------------

	class AIMap;
	class String;   //�����String�Ǵ����������ַ����������Ӵ�

	class String                         //���Ӵ���,��Ϊ����ָ����������ͣ�����ֱ�ӽ���ǳ���ơ�
	{
	public:
		int d;                           //���� 0ΪX+Y- 1ΪX+ 2ΪX+Y+ 3ΪY- �˷����趨��֤���ڽ�����x��y����ʱ��������ĵ����Ǳ��ȱ���
		int length;                      //������
		ptype type;                      //�����ͣ����Ӵ����ǰ��Ӵ�
		int open1, open2;                //�����򿪿ڳ��Ⱥ������򿪿ڳ���
		int startX, startY;              //��ͷ��XY����

		String();
		String(String& str0);
		inline int maxlength();
		inline int OpenNum();
		inline int value();

	};
	class AIMap
	{
	public:
		ChessGame game;                  //���ڵľ��ƣ������ж϶��Ǹ���������ƶ��µ�
		int **value;                     //������ɺ����λ�õļ�ֵ�����ݵ��������Ĳ�ͬ��ֻ�н��м����������壻
		String ****str;                  //ÿ��λ�ö�Ӧ�Ĵ�����һ����ָ�����ά���飬str[x][y][d]
		linkedlist<String> strlist;     //�洢�����������ڱ������д�

		AIMap(const ChessGame&);             
		AIMap(const AIMap& map0);
		~AIMap();

		bool PointLegal(int x, int y);                  //λ���Ƿ�Ϸ�
		bool Same(int x, int y, ptype t);               //ĳ���Ƿ�����һ����ͬ
		bool IsOpnt(int x, int y, ptype t);             //��ĳ����˵����һ���Ƿ񡰲��ɴ�Խ���������з����ӻ�߿�
		bool CanWin(const ChessGame &g, int x, int y, ptype t);
		int BasePointValue(int x, int y, ptype t);      //����ĳ����Ļ�������

		int DeepPointValue(ChessGame game,int x,int y,ptype t,int depth,int OpntMaxValue,int SumValue);      //����ĳ����Ļ�������
		int Put(int x, int y, ptype t);								//��ĳ��������Ӻ����̼�ֵ�ı仯

		int CalcBoardValue_func(int x, int y);						//�������̼�ֵ���Ӻ���
		int CalcBoardValue(ptype);									//�������̼�ֵ
		int CalcBaseValue(ptype);									//�������̼�ֵ
		int CalcDeepValue(ptype t,int depth);							//�������Ϊn������������������̼�ֵ��

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
		if (CanWin(game, x, y, t)) return LinkScore[WinNum];          //�����ֱ��ʤ��������һ���ܴ�ķ���;

#if DEBUGMODE==1//------------------------------------------------
		int CutOrder = 0;
		++SearchNum;
#endif//------------------------------------------------------

		game.cell[x][y] =t;                                           //�����ڴ˵�����
		if (depth == 0)                                               //������Ϊ0����������̼�ֵ�����Ӽ�ֵ=�ּ�ֵ-����ֵ
		{
			map = new AIMap(game);
			value0 = map->CalcBoardValue(t) - value0;
			delete map;
			return value0;
		}
		else                                                                             //�����Ȳ�Ϊ0
		{
			ignet = SetIgnoreNet(game, minIgnoreLength-1);							
			minValue = 0x7FFFFFFF;									//maxValue��ʼֵΪ�����
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
					value =DeepPointValue(game, i, j, t, depth - 1,0-minValue, 0-value0);                //�����ĳ���ӽڵ�ļ�ֵ���Զ�����˵�����м�ֵ��ȡ��
					value = 0 - value;                                                                   
					if (minValue > value) minValue = value;           //��Ϊ�Է����ǻ��ѡ�����ļ�ֵ                
					if (minValue <= OpntMinValue)			          //�����minValueС����һ�����Сֵ����ô�����򲻿���ѡ�������֧�����м�֦��
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

	inline int AIMap::Put(int x, int y, ptype t)             //���㵱��λ��x��y��������ʱ��ֵ�ı仯
	{
		int x1, y1;
		int i, j, x2, y2;
		String *str1, *str2;
		int value0 = 0;                                         //value0�������ĳ�ʼ��ֵ
		int value1 = 0;                                         //value1�������ı仯���ֵ
		if (game.cell[x][y] != t_null) return -1;                //��ѡ�������ʱ��û�б�Ҫ���м���
		for (int d = 0; d <= 3; d++)
		{
			str1 = NULL;                                            //���򸺷������
			x1 = NewX(x, -1, d);
			y1 = NewY(y, -1, d);
			if (PointLegal(x1, y1))                                //ֻ�е��µ�����Ϸ����ż�������
			{
				str1 = str[x1][y1][d];                             //str1Ϊ��õ��´�
				if (str1 != NULL)                                   //���´����ڣ����ܵ����
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
				while (Same(x1, y1, t))  //����+X���򣬼���ͬɫ���ӵ���Ŀ 
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
					while (!IsOpnt(NewX(x, i2, d), NewY(y, i2, d), t)) i2++;             //��������-���򣬵����ֵ��ӻ�߽�����ֹͣ
					j2 = -1;
					while (!IsOpnt(NewX(x, j2, d), NewY(y, j2, d), t)) j2--;            //��������+���򣬵����ֵ��ӻ�߽�����ֹͣ

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

		//-----------------------������-----------------------
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
		
#if DEBUGMODE==1 //----------------------------������--------------------------
		HDC hdc = GetDC(CurrentHwnd());
		int xText = 100;
		int yText = 700;
		float AveCutOrder;
		stdstring sz;
		sz += "�ܹ������ڵ㣺";
		sz += std::to_string(SearchNum);
		sz += "   ��֦������";
		sz+= std::to_string(CutNum);
		TextOut(hdc, xText, yText, TEXT(sz.c_str()), sz.size());

		xText = 100;
		yText = 720;
		sz = "";
		sz += "ƽ����֦����";
		if (CutNum>0) 
		{ 
			AveCutOrder = (float)SumCutOrder / CutNum;
			sz += std::to_string(AveCutOrder);
		}
		else
		{
			sz += "NA";
		}
		sz += "��Լ�֦��";
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
	std::string sz = "���̼�ֵ��";
	sz += (std::to_string(i));
	MessageBox(hwnd, TEXT(sz.c_str()), " ", 0);
}