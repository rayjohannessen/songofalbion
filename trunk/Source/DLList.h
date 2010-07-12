#ifndef DLLIST_H 
#define DLLIST_H

#include <iostream>
using namespace std;

template <typename Type> class DLList;

template <typename Type> 
class DLLIter
{
	friend class DLList<Type>;
	DLList<Type> *list;

	typename DLList<Type>::tNode * curr;

public:

	DLLIter (DLList<Type> &listToIterate)
	{
		list = &listToIterate;
		curr = list->m_pHead;
	}

	void beginHead ()
	{
		curr = list->m_pHead;
	}

	void beginTail ()
	{
		curr = list->m_pTail;
	}

	bool end ()
	{
		if (curr == NULL)
			return true;
		return false;
	}

	Type & current ()
	{
		return curr->data;
	}

	DLLIter<Type> & operator-- ()
	{
		if (curr)
		{
			curr = curr->pPrev;
		}
		return *this;
	}

	DLLIter<Type> & operator++ ()
	{
		if (curr)
		{
			curr = curr->pNext;
		}
		return *this;
	}
};

template <typename Type>
class DLList
{
	friend class DLLIter<Type>;
	
	struct tNode 
	{
		Type data;
		tNode* pNext;
		tNode* pPrev;
	};

	tNode* m_pHead;
	tNode* m_pTail;

	//COPY CTOR
	DLList (const DLList<Type>& list);	
	//Assignment
	DLList<Type>& operator= (const DLList<Type>& list);
public:
	//CTOR
	DLList(void)
	{
		m_pHead = NULL;
		m_pTail = NULL;
	}

	//DTOR
	virtual ~DLList(void)
	{
		clear ();
	}

	void addHead (const Type &item)
	{
		tNode* pNode = new tNode;
		pNode->data = item;

		if (m_pHead)
		{
			pNode->pNext = m_pHead;
			m_pHead->pPrev = pNode;
			m_pHead = pNode;
		}
		else if (!m_pHead)
		{
			pNode->pNext = NULL;
			m_pHead = pNode;
			m_pTail = m_pHead;
		}

		pNode->pPrev = NULL;
	}

	void addTail (const Type &item)
	{
		tNode* pNode = new tNode;
		pNode->data = item;

		if (m_pTail)
		{
			pNode->pPrev = m_pTail;
			m_pTail->pNext = pNode;
			m_pTail = pNode;
		}
		else if (!m_pTail)
		{
			pNode->pPrev = NULL;
			m_pTail = pNode;
			m_pHead = m_pTail;
		}
		pNode->pNext = NULL;
	}

	void insert (DLLIter<Type>&index, const Type &v)
	{
		if (!index.curr)
			return;
		if (index.curr)
		{
			if (index.curr == m_pHead)
			{
				addHead (v);
				index.curr = m_pHead;
			}

			else
			{
				tNode* temp = new tNode;
				temp->data = v;
				temp->pPrev = index.curr->pPrev;
				temp->pNext = index.curr;
				index.curr->pPrev = temp;
				temp->pPrev->pNext = temp;
				index.curr = temp;
			}
		}
	}

	void remove (DLLIter<Type>&index)
	{
		if (!index.curr)
			return;
		if (m_pHead == m_pTail)
		{
			delete m_pTail;
			m_pTail = 0;
			m_pHead = 0;
			index.curr = 0;
		}
		else if (index.curr == m_pHead)
		{
			tNode* temp = m_pHead;
			m_pHead = m_pHead->pNext;
			index.curr = m_pHead;
			delete temp;
			m_pHead->pPrev = NULL;
		}
		else if (index.curr == m_pTail)
		{
			tNode* temp = m_pTail;
			m_pTail = m_pTail->pPrev;
			index.curr = m_pTail;
			delete temp;
			m_pTail->pNext = NULL;
		}
		else
		{
			tNode* temp = index.curr;

			index.curr->pPrev->pNext = index.curr->pNext;
			index.curr->pNext->pPrev = index.curr->pPrev;
			index.curr = index.curr->pNext;
			delete temp;
		}
	}

	void clear ()
	{
		tNode* temp;
		while (m_pHead)
		{
			temp = m_pHead;
			m_pHead = m_pHead->pNext;
			delete temp;
		}
		m_pTail = NULL;
	}
};

#endif DLLIST_H