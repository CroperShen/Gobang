//======================================================================================
//cprlib.h
//Created by CroperShen
//Last modified in Dec.19.2018
//当没有加载某些库时，对一些常用函数的实现
//=========================================================================================



//--------------------------------------------------------------------------------------------
#ifndef _WINDOWS_   
#ifndef _CPR_WINDOWS_
#define _CPR_WINDOWS_
#define min(a,b) ((a)>(b))?(b):(a)             //min max宏，当没有包含window.h原本是无法使用的
#define max(a,b) ((a)>(b))?(a):(b)
#endif
#endif
//------------------------------------------------------------------------------------------
#ifndef _CSTDLIB_
#ifndef _CPR_CSTDLIB_
#define _CPR_CSTDLIB_
inline float abs(float a)                      //float的abs
{
	if (a < 0) return 0 - a;
	return a;
}
#endif
#endif
//-----------------------------------------------------------------------------------------
#ifdef  _STACK_
#ifndef _CPR_STACK_
#define _CPR_STACK_
template <typename T> inline void ClearStack(std::stack<T>& stk)
{
	while (!stk.empty()) stk.pop();
}
template <typename T> T StackPickRandom(std::stack<T> stk)
{
	while (rand() % stk.size() != 0) stk.pop();
	return stk.top();
}
#endif
#endif
//-------------------------------------------------------------------------------------------
#ifndef _CPRLIBFUNC_
#define _CPRLIBFUNC_

float s2f(char* sz);
template <typename T> T* NewArr(int);
template <typename T> T** NewArr(int,int);
template <typename T> T*** NewArr(int,int,int);
template <typename T> T**** NewArr(int,int,int,int);
template <typename T> void DelArr(T*);                    //不需要输入最后一个元素的大小
template <typename T> void DelArr(T**,int);				  //不需要输入最后一个元素的大小
template <typename T> void DelArr(T***,int,int);		  //不需要输入最后一个元素的大小
template <typename T> void DelArr(T****,int,int,int);	  //不需要输入最后一个元素的大小

float s2f(char* sz)                             //字符串（char[])转float
{
	float i = -1;
	float f = 0;
	while (*sz != (char)0)
	{
		if (*sz == '.')
		{
			if (i >= 0) break;
			i = 1;
			sz++;
		}
		if (i < 0)
		{
			f *= 10;
			f += (int)*sz - (int)'0';
		}
		else
		{
			i *= 10;
			f += (float)((int)*sz - (int)'0') / i;
		}
		sz++;
	}
	return f;
}

template <typename T> inline T* NewArr(int i0)
{
	return new T[i0];
}
template <typename T> inline T** NewArr(int i0,int j0)
{
	T** ans = new T*[i0];
	for (int i = 0; i < i0; i++) ans[i] = new T[j0];
	return ans;
}
template <typename T> inline T*** NewArr(int i0, int j0,int k0)
{
	T*** ans = new T**[i0];
	for (int i = 0; i < i0; i++)
	{
		ans[i] = new T*[j0];
		for (int j = 0; j < j0; j++)	ans[i][j] = new T[k0];
	}
	return ans;
}
template <typename T> inline T**** NewArr(int i0, int j0,int k0,int h0)
{
	T**** ans = new T***[i0];
	for (int i = 0; i < i0; i++)
	{
		ans[i] = new T**[j0];
		for (int j = 0; j < j0; j++)
		{
			ans[i][j] = new T*[k0];
			for (int k = 0; k < k0; k++) ans[i][j][k] = new T[h0];
		}
	}
	return ans;
}

template <typename T> inline void DelArr(T* arr)
{
	delete[]arr;
}
template <typename T> inline void DelArr(T** arr,int i0)
{
	for (int i = 0; i < i0; i++) delete[]arr[i];
	delete[]arr;
}
template <typename T> inline void DelArr(T*** arr, int i0,int j0)
{
	for (int i = 0; i < i0; i++)
	{
		for (int j = 0; j < j0; j++) delete[]arr[i][j];
		delete[]arr[i];
	}
	delete[]arr;
}
template <typename T> inline void DelArr(T**** arr, int i0, int j0,int k0)
{
	for (int i = 0; i < i0; i++)
	{
		for (int j = 0; j < j0; j++)
		{
			for (int k = 0; k < k0; k++) delete[]arr[i][j][k];
			delete[]arr[i][j];
		}
		delete[]arr[i];
	}
	delete[]arr;
}

#endif



