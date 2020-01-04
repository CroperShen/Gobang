//linkedlist.h
//Created by CroperShen
//Last modified in Dec.24.2018
//����ṹ����ϰ֮��

#pragma once
#include <initializer_list>
#include "cprlib.h"
#define _LINKEDLIST_H

//������
template <typename T> class link;                  //����ڵ�
template <typename T> class linkedlist;            //����

//�����������

//=======================================================

//��Ķ���
template <typename T> class link    //����ڵ�
{
	friend class linkedlist<T>;
private:
	T Value;              // ֵ��
	link<T> *PreNode;
	link<T> *NextNode;
public:
	link();               //���캯��,
	link(const T&);       //���캯��,
	link(const link<T>&); //���ƹ��캯��
	~link();  //��������

	void SetData(const T&);    //����ֵ      
};

//-----------------------------
template <typename T> class linkedlist //����
{
private:
	link<T> *headernode;  //ͷ�ڵ�,Ϊ��ڵ�,ͷ�ڵ��NextNodeΪ�׽ڵ㣬PreNodeΪβ�ڵ�;
	int l;    //����
protected:
	link<T> *pNode(int);                     //��ȡָ���ڵ㣬����������(����NULL)
	link<T> *pHead();
	link<T> *pTail();
public:

	linkedlist();                                                  //���캯��,����������
	template <typename TArr> linkedlist(const TArr&,int=-1);       //�������鴴������  ���������飬�����С) �������СΪ-1��ΪĬ�����鳤��
	linkedlist(const std::initializer_list<T>&);                   //ʹ�ó�ʼ����������
	linkedlist(linkedlist<T>&);                                    //�������캯��
	~linkedlist();                                                 //�������� 

	void operator=(const linkedlist<T>&);                 //���ظ�ֵ ʹ���Ϊ����
	const bool operator==(const linkedlist<T>&) const;    //������ȣ�ֻ��������ÿһ��Ԫ�ض���ȫ��ͬʱ�����;
	const bool operator!=(const linkedlist<T>&) const ;   //�������;
	T& operator[](int);                                   //�����±���ţ���ֱ��������һ��ʹ��


	const bool empty() const;            //�Ƿ��ǿ�����
	const int size() const;              //��ȡ������
	const T& value(int)const;            //��ȡ����λ�õ�ֵ
	const T& head()const;                //��ȡ��һ���ڵ��ֵ
	const T& tail()const;                //��ȡ���һ���ڵ��ֵ


	T& Append(const T&);             //��β�����һ��ֵ��������ӵ�ֵ
	T& Append();                     //��β�����һ��ֵ��������ӵ�ֵ
	T& Insert(const T& ,int);        //�ڸ���λ�����һ��ֵ������ֵ˳�ӣ�������ӵ�ֵ 
	T& Insert(int);                  //�ڸ���λ�����һ��ֵ������ֵ˳�ӣ�������ӵ�ֵ
	void SetHead(const T&);            //�ı�ͷ�ڵ��ֵ,����ͷ�ڵ��ֵ
	void SetTail(const T&);            //�ı�β�ڵ��ֵ,����β�ڵ��ֵ
	void SetValue(int,const T&);       //�ı����λ�õ�ֵ,���ظ���λ�õ�ֵ
	const bool Delete(int);                //ɾ������λ�õ�ֵ���糬�޻᷵��ʧ��
	const bool Delete(T*);                 //ɾ��ָ��ָ���ֵ����û�л᷵��ʧ��

	void Clear();                          //�������
	linkedlist<T>& Turn();                 //ת�������ױ�Ϊββ��Ϊ�ף�
	void SetSize(int);                    //���ó��ȣ�����Ļᱻ�ص�������Ჹ��Ĭ�Ͻڵ㡣

	T *array() const;                   //��Ԫ���������������У��������飬�Ḵ��Ԫ��

};

//��������

//========================================================================================================

//������link<T>������ڵ�)�ĺ�������
template <typename T> link<T>::link()              //���캯��
{
	PreNode = NULL;
	NextNode = NULL;
}

template <typename T> link<T>::link(const T &data) //���캯��
{
	Value = data;
	PreNode = NULL;
	NextNode = NULL;
}

template <typename T> link<T>::link(const link<T> &lk)  //�������캯��
{
	Value = lk.Value;
	PreNode = NULL;
	NextNode = NULL;
}

template <typename T> link<T>::~link()        //��������
{
	PreNode = NULL;
	NextNode = NULL;
}

template <typename T> void link<T>::SetData(const T &data)   //����ֵ     
{
	Value = data;
}
//--------

//������linkedlisk<T>����ĺ�������

	//==================================
	//����
template <typename T> linkedlist<T>::linkedlist() //���캯��,����������
{
	headernode = new link<T>;
	headernode->PreNode = headernode;
	headernode->NextNode = headernode;

	l = 0;
}

template <typename T> template <typename TArr> linkedlist<T>::linkedlist(const TArr& Arr,int n)  //�������鴴������  ���������飬�����С) �������СΪ-1��ΪĬ�����鳤��
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

	l = n;                                                             //����=�趨����
}

template <typename T> linkedlist<T>::linkedlist(const std::initializer_list<T>& initlist) //ʹ�ó�ʼ����������
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
		p = new link<T>((T)*tp);                              //������ڵ��ֵǿ��ת��ΪT
		p->PreNode = pPre;
		pPre->NextNode = p;
		pPre = p;
	}

	headernode->PreNode = p;
	p->NextNode = headernode;
	l = initlist.size();
}

template <typename T> linkedlist<T>::linkedlist(linkedlist<T>& list)   // �������캯��
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

	l = l2;                                                         //����=����������ĳ���
}
//-----------
template <typename T> linkedlist<T>::~linkedlist()                   //��������  
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

template <typename T> void linkedlist<T>::operator=(const linkedlist<T>& list)  //���ظ�ֵ ʹ���Ϊ����
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
template <typename T> T& linkedlist<T>::Append(const T& data)          //��β�����һ��ֵ
{
	link<T> *p = new link<T>(data);
	p->PreNode = headernode->PreNode;
	p->NextNode = headernode;
	headernode->PreNode->NextNode = p;
	headernode->PreNode = p;

	l++;
	return p->Value;
}

template <typename T> T& linkedlist<T>::Append()          //��β�����һ��ֵΪĬ�ϵĽڵ�
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
template <typename T> T& linkedlist<T>::Insert(const T& data,int i)   //�ڸ���λ�����һ��ֵ������ֵ˳��
{
	link<T> *p = new link<T>(data);
	link<T> *pPre;

	int z = size() - 1;
	int x = 0;
	if (i == z + 1) return Append(data);
	i = min(z, max(x, i));                //i����λ��0����󳤶�-1֮��

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

	l++;                                          //����+1
	return p->Value;
}

template <typename T> T& linkedlist<T>::Insert(int i)   //�ڸ���λ�����һ��ֵΪĬ�ϵĽڵ㣬����ֵ˳��
{
	link<T> *p = new link<T>;
	link<T> *pPre;

	int z = size() - 1;
	int x = 0;
	if (i == z + 1) return Append();
	i = min(z, max(x, i));                //i����λ��0����󳤶�-1֮��

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

	l++;                                          //����+1
	return p->Value;
}

template <typename T> void linkedlist<T>::SetHead(const T& t)            //�ı�ͷ�ڵ�ֵ
{
	if (size() == 0) return;
	pHead()->Value= t;
}

template <typename T> void linkedlist<T>::SetTail(const T&t)             //�ı�β�ڵ��ֵ
{
	if (size() == 0) return;
	pTail()->Value = t;
}
template <typename T> void linkedlist<T>::SetValue(int i,const T&t)        //�ı����λ�õ�ֵ
{
	if (size() == 0) return;
	if ((i < 0) || (i >= size())) return;
	pNode(i)->Value = t;
}
template <typename T> const bool linkedlist<T>::Delete(int i) //ɾ������λ�õ�ֵ���糬�޻᷵��ʧ��
{
	link<T> *p = pNode(i);
	if (p == NULL) return false;                                   //��û�д˽ڵ㷵��ʧ��

	p->PreNode->NextNode = p->NextNode;
	p->NextNode->PreNode = p->PreNode;
	delete p;

	l--;                                                          //����-1
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

template <typename T> void linkedlist<T>::Clear() //�������
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
	l = 0;                                                             //��������Ϊ0
}

template <typename T> linkedlist<T>& linkedlist<T>::Turn()                  //ת�������ױ�Ϊββ��Ϊ�ף�
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

template <typename T> void linkedlist<T>::SetSize(int size1)                    //���ó��ȣ�����Ļᱻ�ص�������Ჹ��Ĭ�Ͻڵ㡣
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

template <typename T> const bool linkedlist<T>::operator==(const linkedlist<T>& list) const   //�������
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


template <typename T> const bool linkedlist<T>::operator!=(const linkedlist<T>& list) const //�������
{
	return (!operator==(list));
}
template <typename T> T& linkedlist<T>::operator[](int i)              //�����±��������ʹ����������һ������
{
	return pNode(i)->Value;
}

template <typename T>  link<T>*linkedlist<T>::pNode(int i)           //����ָ���ض��ڵ��ָ��
{
	link<T> *p;
	if (i < 0 || i >= size()) return NULL;                         //������޷���NULL

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

template <typename T> link<T> *linkedlist<T>::pHead()                                                      //����ָ���׽ڵ��ָ��
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