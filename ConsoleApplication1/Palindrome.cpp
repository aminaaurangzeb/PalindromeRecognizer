#include "stdafx.h"
#include <iostream>
#include <conio.h>
#include "DLList.h"
using namespace std;

template<class T>
bool isPal(DLList<T> &list,node<T>* s,node<T>* e){
	if(s==e)
		return true;
	else{
		if(s->getdata()!=e->getdata())
			return false;
		return isPal(list,s->getnext(),e->getprev());
	}
}

int main(){
	DLList<int> list;
	list.addToTail(3);
	list.addToTail(2);
	list.addToTail(2);
	list.addToTail(3);
	cout<< isPal(list,list.begin().getiptr(),list.end().getiptr());
	_getch();
}

