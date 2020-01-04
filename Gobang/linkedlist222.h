//linkedlist.h
//Created by CroperShen
//Last modified in Dec.24.2018
//链表结构，练习之做

#pragma once
#include <initializer_list>
#include "cprlib.h"
#define _LINKEDLIST_H

//声明类
template <typename T> class link;                  //链表节点
template <typename T> class linkedlist;            //链表

//类的声明结束

//=======================================================

//类的定义
template <typename T> class link    //链表节点
{
	friend class linkedlist<T>;
private:
	T Value;              // 值域
	link<T> *PreNode;
	link<T> *NextNode;
public:
	link();               //构造函数,
	link(const T&);       //构造函数,
	link(const link<T>&); //复制构造函数
	~link();  //析构函数

	void SetData(const T&);    //设置值      
};

//-----------------------------
template <typename T> class linkedlist //链表
{
private:
	link<T> *headernode;  //头节点,为虚节点,头节点的NextNode为首节点，PreNode为尾节点;
	int l;    //长度
protected:
	link<T> *pNode(int);                     //获取指定节点，限制上下限(返回NULL)
	link<T> *pHead();
	link<T> *pTail();
public:

	linkedlist();                                                  //构造函数,创建空链表
	template <typename TArr> linkedlist(const TArr&,int=-1);       //根据数组创建链表  参数（数组，链表大小) 如链表大小为-1则为默认数组长度
	linkedlist(const std::initializer_list<T>&);                   //使用初始化表构造链表
	linkedlist(linkedlist<T>&);                                    //拷贝构造函数
	~linkedlist();                                                 //析构函数 

	void operator=(const linkedlist<T>&);                 //重载赋值 使其变为复制
	const bool operator==(const linkedlist<T>&) const;    //链表相等，只有在链表每一个元素都完全相同时才相等;
	const bool operator!=(const linkedlist<T>&) const ;   //链表不相等;
	T& operator[](int);                                   //重载下标符号，能直接像数组一样使用


	const bool empty() const;            //是否是空链表
	const int size() const;              //获取链表长度
	const T& value(int)const;            //获取给定位置的值
	const T& head()const;                //获取第一个节点的值
	const T& tail()const;                //获取最后一个节点的值


	T& Append(const T&);             //于尾部添加一个值，返回添加的值
	T& Append();                     //于尾部添加一个值，返回添加的值
	T& Insert(const T& ,int);        //于给定位置添加一个值，其他值顺延，返回添加的值 
	T& Insert(int);                  //于给定位置添加一个值，其他值顺延，返回添加的值
	void SetHead(const T&);            //改变头节点的值,返回头节点的值
	void SetTail(const T&);            //改变尾节点的值,返回尾节点的值
	void SetValue(int,const T&);       //改变给定位置的值,返回给定位置的值
	const bool Delete(int);                //删除给定位置的值，如超限会返回失败
	const bool Delete(T*);                 //删除指针指向的值，如没有会返回失败

	void Clear();                          //清空链表
	linkedlist<T>& Turn();                 //转置链表，首变为尾尾变为首；
	void SetSize(int);                    //设置长度，多余的会被截掉，少则会补充默认节点。

	T *array() const;                   //将元素重新填入数组中，返回数组，会复制元素

};

//声明结束

//========================================================================================================

//以下是link<T>（链表节点)的函数定义
template <typename T> link<T>::link()              //构造函数
{
	PreNode = NULL;
	NextNode = NULL;
}

template <typename T> link<T>::link(const T &data) //构造函数
{
	Value = data;
	PreNode = NULL;
	NextNode = NULL;
}

template <typename T> link<T>::link(const link<T> &lk)  //拷贝构造函数
{
	Value = lk.Value;
	PreNode = NULL;
	NextNode = NULL;
}

template <typename T> link<T>::~link()        //析构函数
{
	PreNode = NULL;
	NextNode = NULL;
}

template <typename T> void link<T>::SetData(const T &data)   //设置值     
{
	Value = data;
}
//--------

//以下是linkedlisk<T>链表的函数定义

	//==================================
	//定义
template <typename T> linkedlist<T>::linkedlist() //构造函数,创建空链表
{
	headernode = new link<T>;
	headernode->PreNode = headernode;
	headernode->NextNode = headernode;

	l = 0;
}

template <typename T> template <typename TArr> linkedlist<T>::linkedlist(const TArr& Arr,int n)  //根据数组创建链表  参数（数组，链表大小) 如链表大小为-1则为默认数组长度
{
	link<T> *p, *pPre;
	int l2 = sizeof(Arr) / sizeof(Arr[0]);

	if ((n < 0) || (n > l2)) n = l2;

	headernode = new link<T>;
	p = headernode;
	for (int i = 0;i < n;i++)
	{
		pPre = p;
		p = new link<T>(Arr[i]);
		p->PreNode = pPre;
		pPre->NextNode = p;
	}

	headernode->PreNode = p;
	p->NextNode = headernode;

	l = n;                                                             //长度=设定长度
}

template <typename T> linkedlist<T>::linkedlist(const std::initializer_list<T>& initlist) //使用初始化表构造链表
{
	link<T> *p, *pPre;
	//T tTemp;

	headernode = new link<T>;

	if (initlist.size() <= 0)
	{
		headernode->PreNode = headernode;
		headernode->NextNode = headernode;
		l = 0;
		return;
	}

	p=pPre = headernode;
	for (auto tp = initlist.begin(); tp != initlist.end(); tp++)
	{
		//tTemp = *tp;
		//p = new link<T>(tTemp);
		p = new link<T>((T)*tp);                              //将链表节点的值强制转换为T
		p->PreNode = pPre;
		pPre->NextNode = p;
		pPre = p;
	}

	headernode->PreNode = p;
	p->NextNode = headernode;
	l = initlist.size();
}

template <typename T> linkedlist<T>::linkedlist(linkedlist<T>& list)   // 拷贝构造函数
{
	link<T> *p,*pPre,*p2;
	int l2 = list.size();

	headernode = new link<T>;
	p = headernode;
	p2 = list.headernode;
	for (int i = 0; i < list.size(); i++) 
	{
		pPre = p;
		p2 = p2->NextNode;
		p = new link<T>(*p2);
		p->PreNode = pPre;
		pPre->NextNode = p;
	}
	
	p->NextNode = headernode;
	headernode->PreNode = p;

	l = l2;                                                         //长度=被复制链表的长度
}
//-----------
template <typename T> linkedlist<T>::~linkedlist()                   //析构函数  
{
	link<T> *p;

	p = headernode->NextNode;
	while (p != headernode)
	{
		p = p->NextNode;
		delete p->PreNode;
	}
	delete headernode;
}

template <typename T> void linkedlist<T>::operator=(const linkedlist<T>& list)  //重载赋值 使其变为复制
{
	link<T> *p1, *p2,*pPre;

	p1 = headernode->NextNode;
	p2 = list.headernode->NextNode;
	while ((p1!= headernode) && (p2!= list.headernode))
	{
		p1->Value = p2->Value;
		p1 = p1->NextNode;
		p2 = p2->NextNode;
	}

	if (size()<list.size())
	{ 
		pPre = headernode->PreNode;
	    while (p2 != list.headernode)
	    {
			p1 = new link<T>(*p2);
			p1->PreNode = pPre;
			pPre->NextNode = p1;
			pPre = p1;
			p2 = p2->NextNode;
	    }
		headernode->PreNode = p1;
		p1->NextNode = headernode;
	}
	else if (size() > list.size())
	{
		headernode->PreNode = p1->PreNode;
		p1->PreNode->NextNode = headernode;
		while (p1 != headernode)
		{
			pPre = p1;
			p1 = p1->NextNode;
			delete pPre;
		}
	}
	l = list.size();
}
//---------------
template <typename T> T& linkedlist<T>::Append(const T& data)          //于尾部添加一个值
{
	link<T> *p = new link<T>(data);
	p->PreNode = headernode->PreNode;
	p->NextNode = headernode;
	headernode->PreNode->NextNode = p;
	headernode->PreNode = p;

	l++;
	return p->Value;
}

template <typename T> T& linkedlist<T>::Append()          //于尾部添加一个值为默认的节点
{
	link<T> *p = new link<T>;
	p->PreNode = headernode->PreNode;
	p->NextNode = headernode;
	headernode->PreNode->NextNode = p;
	headernode->PreNode = p;

	l++;
	return p->Value;
}
//----------------------
template <typename T> T& linkedlist<T>::Insert(const T& data,int i)   //于给定位置添加一个值，其他值顺延
{
	link<T> *p = new link<T>(data);
	link<T> *pPre;

	int z = size() - 1;
	int x = 0;
	if (i == z + 1) return Append(data);
	i = min(z, max(x, i));                //i必须位于0到最大长度-1之间

	if (i == 0)
	{
		pPre = headernode;
	}
	else
	{
		pPre = pNode(i - 1);
	}

	p->PreNode = pPre;
	p->NextNode = pPre->NextNode;

	p->PreNode->NextNode = p;
	p->NextNode->PreNode = p;

	l++;                                          //长度+1
	return p->Value;
}

template <typename T> T& linkedlist<T>::Insert(int i)   //于给定位置添加一个值为默认的节点，其他值顺延
{
	link<T> *p = new link<T>;
	link<T> *pPre;

	int z = size() - 1;
	int x = 0;
	if (i == z + 1) return Append();
	i = min(z, max(x, i));                //i必须位于0到最大长度-1之间

	if (i == 0)
	{
		pPre = headernode;
	}
	else
	{
		pPre = pNode(i - 1);
	}

	p->PreNode = pPre;
	p->NextNode = pPre->NextNode;

	p->PreNode->NextNode = p;
	p->NextNode->PreNode = p;

	l++;                                          //长度+1
	return p->Value;
}

template <typename T> void linkedlist<T>::SetHead(const T& t)            //改变头节点值
{
	if (size() == 0) return;
	pHead()->Value= t;
}

template <typename T> void linkedlist<T>::SetTail(const T&t)             //改变尾节点的值
{
	if (size() == 0) return;
	pTail()->Value = t;
}
template <typename T> void linkedlist<T>::SetValue(int i,const T&t)        //改变给定位置的值
{
	if (size() == 0) return;
	if ((i < 0) || (i >= size())) return;
	pNode(i)->Value = t;
}
template <typename T> const bool linkedlist<T>::Delete(int i) //删除给定位置的值，如超限会返回失败
{
	link<T> *p = pNode(i);
	if (p == NULL) return false;                                   //如没有此节点返回失败

	p->PreNode->NextNode = p->NextNode;
	p->NextNode->PreNode = p->PreNode;
	delete p;

	l--;                                                          //长度-1
	return true;
}

template<typename T>

inline const bool linkedlist<T>::Delete(T *tp)
{
	link<T> *p;
	p = headernode->NextNode;
	while (p != headernode)
	{
		if (tp == &(p->Value))
		{
			p->PreNode->NextNode = p->NextNode;
			p->NextNode->PreNode = p->PreNode;
			delete p;
			l--;
			return true;
		}
		p = p->NextNode;
	}
	return false;
}

template <typename T> void linkedlist<T>::Clear() //清空链表
{
	link<T> *p;
	p = headernode->NextNode;
	while (p != headernode)
	{
		p = p->NextNode;
		delete p->PreNode;
	}

	headernode->PreNode = headernode;
	headernode->NextNode = headernode;
	l = 0;                                                             //长度设置为0
}

template <typename T> linkedlist<T>& linkedlist<T>::Turn()                  //转置链表，首变为尾尾变为首；
{
	link<T> *p, *pTemp;

	p = headernode;
	do
	{
		pTemp = p->PreNode;
		p->PreNode = p->NextNode;
		p->NextNode = pTemp;
		
		p = p->PreNode;
	} while (p != headernode);

	return *this;
}

template <typename T> void linkedlist<T>::SetSize(int size1)                    //设置长度，多余的会被截掉，少则会补充默认节点。
{
	link<T> p, *pPre;

	if (l < size1)
	{
		p = headernode->PreNode;
        
		for (int i = l; i < size1; i++)
		{
			p->NextNode = new link<T>;
			p->NextNode->PreNode = p;
			p = p->NextNode;
		}
		headernode->PreNode = p;
		p->NextNode = headernode;
	}
	else if (l>size1)
	{
		p = headernode->PreNode;
		for (int i = size1; i < l; i++)
		{
			p = p->PreNode;
			delete p->NextNode;
		}
		headernode->PreNode = p;
		p->NextNode = headernode;
	}
	l = size1;
}

template <typename T> const bool linkedlist<T>::operator==(const linkedlist<T>& list) const   //链表相等
{
	link<T> *p1, *p2;
	if (size() != list.size()) return false;
	p1 = headernode->NextNode;
	p2 = list.headernode->NextNode;

	while (p1 != headernode)
	{
		if (p1->Value != p2->Value) return false;
		p1 = p1->NextNode;
		p2 = p2->NextNode;
	}
	return true;
}


template <typename T> const bool linkedlist<T>::operator!=(const linkedlist<T>& list) const //链表不相等
{
	return (!operator==(list));
}
template <typename T> T& linkedlist<T>::operator[](int i)              //重载下标运算符，使其能像数组一样访问
{
	return pNode(i)->Value;
}

template <typename T>  link<T>*linkedlist<T>::pNode(int i)           //返回指向特定节点的指针
{
	link<T> *p;
	if (i < 0 || i >= size()) return NULL;                         //如果超限返回NULL

	if (2 * i <= size())
	{
		p = headernode->NextNode;
		for (int j = 1; j <= i; j++)
		{
			p = p->NextNode;
		}
	}
	else
	{
		p = headernode->PreNode;
		for (int j = size() - 2; j >= i; j--)
		{
			p = p->PreNode;
		}
	}
	return p;
}

template <typename T> link<T> *linkedlist<T>::pHead()                                                      //返回指向首节点的指针
{
	if (size() > 0) return headernode->NextNode;
	return NULL;
}

template <typename T> link<T> *linkedlist<T>::pTail()
{
	if (size() > 0) return headernode->PreNode;
	return NULL;
}

template <typename T> const inline bool linkedlist<T>::empty() const
{
	return (size()==0);
}

template <typename T> const inline int linkedlist<T>::size() const
{
	return l;
}

template <typename T> inline const T& linkedlist<T>::value(int i) const
{
	return const_cast<linkedlist<T>*>(this)->pNode(i)->Value;
}

template <typename T> inline const T& linkedlist<T>::head() const
{
	return headernode->NextNode->Value;
}

template <typename T> inline const T& linkedlist<T>::tail() const
{
	return headernode->PreNode->Value;
}

template <typename T> T *linkedlist<T>::array() const
{
	T *a;
	link<T> *p;

	if (size() == 0) return NULL;
	a = new T[size()];
	
	p = headernode->NextNode;
	for (int i = 0; i < size();i++)
	{
		a[i] = p->Value;
	}
	return a;
}