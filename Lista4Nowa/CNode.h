#pragma once
#include <string>
#include <vector>
template <typename T>class CNode
{
private:
	//domsylna pojemnosc
	static int const iDeafultCapacity;
	//operator
	std::string chOperand;
	//rodzic
	CNode<T>* parent;
	//dzieci jako wektor wskaznikow
	std::vector<CNode<T>*> cChildren;
	//ilosc dzeci
	int iNumOfChildren;
	//max ilosc dzieci
	int iNumOfMaxChildren;
public:
	//wyjasnienia funkcji w pliku cpp//
	CNode<T>();
	~CNode<T>();
	CNode(CNode<T>& otherNode);
	CNode(std::string cOp,int fill,std::string fillerS ,CNode<T>* cParent);
	void addChildren(CNode<T>* cChild);
	std::vector<CNode<T>*> getChildren()
	{
		return cChildren;
	}
	CNode<T>& operator=(const CNode<T>& otherNode);
	std::string sGetKnownType();
	template <typename T>
	friend class CTree;
};
template <typename T>
const int CNode<T>::iDeafultCapacity = 2;
template <typename T>
CNode<T>::CNode()
{
	//domyslne wartosci
	parent = NULL;
	//rezerwujemy 2 miejsca w wekotrze dzieci
	cChildren.reserve(iDeafultCapacity);
	chOperand = "";
	iNumOfChildren = 0;
	iNumOfMaxChildren = 0;
}
template <typename T>
CNode<T>::CNode(CNode<T>& otherNode)
{
	this->parent = otherNode.parent;
	this->cChildren.clear();
	this->chOperand = otherNode.chOperand;
	this->iNumOfChildren = otherNode.iNumOfChildren;
	this->iNumOfMaxChildren = otherNode.iNumOfMaxChildren;
	for (int i = 0; i < cChildren.size(); i++)
	{
		//przepisz dzieci
		if (i < otherNode.cChildren.size())
		{
			this->cChildren.push_back(otherNode.cChildren[i]);
		}
	}
}
template <typename T>
CNode<T>::~CNode()
{
	delete parent;
}
template <typename T>
CNode<T>::CNode(std::string cOp,int fill,std::string fill2, CNode* cParent)
{
	parent = cParent;
	chOperand = cOp;
	iNumOfChildren = 0;
	iNumOfMaxChildren = 0;
	cChildren.reserve(iDeafultCapacity);
}
template <typename T>
void CNode<T>::addChildren(CNode<T>* cChild)
{
	//dodaj dziecko
	cChildren.push_back(cChild);
	cChild->parent = this;
	//zwieksz informacje o liczbie dzieci
	iNumOfChildren++;
}
template <typename T>
CNode<T>& CNode<T>::operator=(const CNode<T>& otherNode)
{
	//skopiuj zmienne
	this->parent = otherNode.parent;
	this->cChildren.clear();
	this->chOperand = otherNode.chOperand;
	this->iNumOfChildren = otherNode.iNumOfChildren;
	this->iNumOfMaxChildren = otherNode.iNumOfMaxChildren;
	for (int i = 0; i < cChildren.size(); i++)
	{
		//skopiuj dzieci
		if (i < otherNode.cChildren.size())
		{
			this->cChildren.push_back(otherNode.cChildren[i]);
		}
	}
	//zwroc nowego Noda
	return *this;
}
template<typename T>
std::string CNode<T>::sGetKnownType()
{
	return "";
}
template <> inline
std::string CNode<int>::sGetKnownType()
{
	return "int";
}
template<> inline
std::string CNode<double>::sGetKnownType()
{
	return "double";
}
template<> inline
std::string CNode<std::string>::sGetKnownType()
{
	return "string";
}



