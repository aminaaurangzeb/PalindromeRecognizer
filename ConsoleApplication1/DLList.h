#pragma once
#include "stdafx.h"
#include<iostream>
#include<conio.h>
#include<cstdlib>
using namespace std;

//Data Structures Assignment 1

template <class T>
class node{
	//template class of node for doubly linked list
private:
	T data;
	node<T>* next;
	node<T>* prev;
	//pointers to next and previous nodes in the list
public:
	node(const T& value, node<T>* prv = nullptr, node<T>* nxt = nullptr);
	node<T>* getprev();
	node<T>* getnext();
	void setprev(node<T>* prv);
	void setnext(node<T>* nxt);
	T getdata();
	void setdata(const T& dat);
	~node();
};

template<class T>
node<T>::node(const T& value, node<T>* prv = nullptr, node<T>* nxt = nullptr){
	//T& so that when any class is passed then it doesnot make a copy
	//Incase prv or nxt not passed then by default both set to nullptr
	data = value;
	//in case T is any class then = operator will have to be overloaded in 
	//that class to avoid shallow copy
	prev = prv;
	next = nxt;
}


template<class T>
node<T>* node<T>::getprev(){
	//prev getter
	return prev;
}

template<class T>
node<T>* node<T>::getnext(){
	//next getter
	return next;
}

template<class T>
void node<T>::setprev(node<T>* prv){
	//prev setter
	prev = prv;
}

template<class T>
void node<T>::setnext(node<T>* nxt){
	//next setter
	next = nxt;
}

template<class T>
T node<T>::getdata(){
	//data getter
	return data;
}

template<class T>
void node<T>::setdata(const T& dat){
	//data setter
	data = dat;
}

template<class T>
node<T>::~node(){
	//destructor
	//it is a recursive destructor which deletes the whole list
	//when only head is deleted in the list's destructor
	if (next != NULL)
		delete next;
	else
		next = nullptr;
	//all previous nodes are by now deleted as well 
	//since we deleted the node next to every node
	prev = nullptr;
	//prev and next set to nullptr to avoid dangling pointers
}
template<class T>
class DLList{
	//doubly linked list class
private:
	node<T>* head;
	//pointer to the starting node
	node<T>* tail;
	//pointer to the last node
	int size;
	//maintains the size of the list
	class ListIterator;
	//Iterator class defined outside the class
	//It allows the user to iterate through the list
	class ReverseListIterator;
	//Reverse Iterator class defined outside the class
	//It allows the user to iterate through the list in the reverse direction
public:
	typedef ListIterator Iterator;
	typedef ReverseListIterator Reverse_Iterator;
	//ListIterator and ReverseListIterator defined as a type so that it can be used 
	//while it remains private data members of DLList class

	DLList(int s = 0, node<T>* hd = nullptr, node<T>* tl = nullptr);
	void addToTail(const T & element);
	void addToHead(const T & element);
	void deletefromTail();
	void deletefromHead();
	int getsize(){return size;}
	node<T>* find(const T& val);
	void remove(Iterator ite);
	void insertBefore(Iterator ite, const T & newelement);
	void insertAfter(Iterator ite, const T & newelement);
	void Swap(Iterator p, Iterator q);
	void SortList(Iterator p, const Iterator q);
	void printlist();
	void read(char* filename);

	Iterator begin(){
		//beginning point of Iterator is head
		Iterator i(head);
		return i;
	}
	Iterator end(){
		//ending point of Iterator is head
		Iterator i(tail);
		return i;
	}
	Reverse_Iterator reverse_begin(){
		//beginning point of Reverse_Iterator is tail
		Reverse_Iterator i(tail);
		return i;
	}
	Reverse_Iterator reverse_end(){
		//ending point of Reverse_Iterator is head
		Reverse_Iterator i(head);
		return i;
	}

	~DLList();
};

template<class T>
DLList<T>::DLList(int s = 0, node<T>* hd = nullptr, node<T>* tl = nullptr){
	size = s;
	head = hd;
	tail = tl;
}

template<class T>
void DLList<T>::read(char* filename){
	//reads the file ignoring whatever is at first line
	ifstream fin(filename);
	if (!fin)
		//if file can not be read for some reason
		cout << filename << " not found!";
	else{
		T tempdata;
		char* junk = new char[100];
		//junk created on heap so that can be deleted after reading the file
		//since the file format told to us requires the first line of the file
		//to be ignored so we keep a junk array which will temporarily 
		//keep the junk stuff read from the file which will be deleted later
		//it is necessary to read the first line in order to move to the second line
		int i = 0;
		//It maintains the line number being read from the file
		//so that we know when we are reading the first line which has to be read in junk array
		while (!fin.eof()){
			if (i == 0){
				//special case of ignoring the first line of file
				fin.getline(junk, 100);
			}
			if (i > 0){
				//reads data normally
				fin >> tempdata;
				//In case if T is any other class then it will use the overloaded
				//operator >> of that class
				this->addToTail(tempdata);
			}
			i++;
			//incrementing line number
		}
		delete junk;
		junk = nullptr;
		//deleting the junk array with the first line of file
	}
}

template<class T>
void DLList<T>::addToTail(const T & newdata){
	if (head == nullptr && tail == nullptr){
		//empty list
		head = tail = new node<T>(newdata, nullptr, nullptr);
	}
	else{
		tail->setnext(new node<T>(newdata, tail, nullptr));
		//tail's next updated with new node
		tail = tail->getnext();
		//tail updated with new node
	}
	size++;
}
template<class T>
void DLList<T>::addToHead(const T & newdata){
	if (head == nullptr && tail == nullptr){
		//empty list
		head = tail = new node<T>(newdata, nullptr, nullptr);
	}
	else{
		head->setprev(new node<T>(newdata, nullptr, head));
		//head's prev updated with the new node
		head = head->getprev();
		//head updated with new node
	}
	size++;
}

template<class T>
void DLList<T>::deletefromHead(){
	if (head != nullptr && tail != nullptr){
		node<T>* curr = head;
		//curr points to the node to be deleted
		head = head->getnext();
		//head updated with next of what will be deleted
		curr->setnext(nullptr);
		curr->setprev(nullptr);
		//next and prev of what will be deleted is set to nullptr
		//so that when our recursive node destructor is called
		//we do not delete the nodes which are to be maintained in the list
		delete curr;
		head->setprev(nullptr);
		//prev of head updated to null
		size--;
	}
}

template<class T>
void DLList<T>::deletefromTail(){
	if (head != nullptr && tail != nullptr){
		node<T>* curr = tail;
		//curr points to the node to be deleted
		tail = tail->getprev();
		//tail is now the prev node of old tail
		curr->setnext(nullptr);
		curr->setprev(nullptr);
		//next and prev of what will be deleted is set to nullptr
		//so that when our recursive node destructor is called
		//we do not delete the nodes which are to be maintained in the list
		delete curr;
		tail->setnext(nullptr);
		//next of tail set to null
	}
}

template<class T>
void DLList<T>::insertBefore(Iterator ite, const T & newdata){
	node<T>* curr = ite.getiptr();
	//now we have our curr and on the basis of it we will insert the new node
	if (curr == nullptr && tail == nullptr && head == nullptr){
		//empty list
		head = tail = new node<T>(newdata, nullptr, nullptr);
	}
	else if (curr == nullptr){
		//add at the end
		tail->setnext(new node<T>(newdata, tail, nullptr));
		tail = tail->getnext();
	}
	else if (curr->getprev() == nullptr){
		//add at the start
		head->setprev(new node<T>(newdata, nullptr, head));
		head = head->getprev();
	}
	else{
		//index is in between two nodes
		node<T>* temp = curr->getprev();
		temp->setnext(new node<T>(newdata, curr->getprev(), curr));
		curr->setprev(temp->getnext());
	}
	size++;
	//size of list incremented
	curr = nullptr;
	//to avoid dangling pointers
}

template<class T>
void DLList<T>::insertAfter(Iterator ite, const T & newdata){
	node<T>* curr = ite.getiptr();
	//now we have our curr and on the basis of it we will insert the new node
	if (curr == nullptr && tail == nullptr && head == nullptr){
		//empty list
		head = tail = new node<T>(newdata, nullptr, nullptr);
	}
	else if (curr->getnext() == nullptr){
		//add at the end
		tail->setnext(new node<T>(newdata, tail, nullptr));
		tail = tail->getnext();
	}
	else if (curr == nullptr){
		//add at the start
		head->setprev(new node<T>(newdata, nullptr, head));
		head = head->getprev();
	}
	else{
		//index is in between two nodes
		node<T>* temp = curr->getnext();
		curr->setnext(new node<T>(newdata, curr, curr->getnext()));
		temp->setprev(curr->getnext());
	}
	size++;
	//size of list incremented
	curr = nullptr;
	//curr set to nullptr to avoid dangling pointer
}

template<class T>
node<T>* DLList<T>::find(const T& val){
	//returns the pointer to the node having val as its data
	node<T>* temp = head;
	for (; temp != nullptr; temp = temp->getnext()){
		if (temp->getdata() == val)
			//incase T is a class the overloaded operator == will be called
			return temp;
	}
	return nullptr;
	//it is executed only if val not found in the list
}

template<class T>
void DLList<T>::remove(Iterator ite){
	node<T>* curr = ite.getiptr();
	//curr will point to the index where new node has to be created.
	//now we have our curr and on the basis of it we will insert the new node
	if (curr != nullptr && tail != nullptr){
		//makes sure that keydata exists in the list and the list is not empty
		if (curr == tail){
			//end
			tail = tail->getprev();
			curr->setnext(nullptr);
			curr->setprev(nullptr);
			//next and prev of what will be deleted is set to nullptr
			//so that when our recursive node destructor is called
			//we do not delete the nodes which are to be maintained in the list
			delete curr;
			tail->setnext(nullptr);
		}
		else if (curr->getprev() == nullptr){
			//start
			head = head->getnext();
			curr->setnext(nullptr);
			curr->setprev(nullptr);
			//next and prev of what will be deleted is set to nullptr
			//so that when our recursive node destructor is called
			//we do not delete the nodes which are to be maintained in the list
			delete curr;
			head->setprev(nullptr);
		}
		else{
			//index is in between two nodes
			curr->getprev()->setnext(curr->getnext());
			curr->getnext()->setprev(curr->getprev());
			//link connections updated accordingly
			curr->setnext(nullptr);
			curr->setprev(nullptr);
			//next and prev of what will be deleted is set to nullptr
			//so that when our recursive node destructor is called
			//we do not delete the nodes which are to be maintained in the list
			delete curr;
		}
		size--;
		//decrementing size of array
	}
	curr = nullptr;
	//to avoid dangling pointer
}

template<class T>
void DLList<T>::Swap(Iterator p, Iterator q){
	if (p.getiptr() == head)
		head = q.getiptr();
	if (p.getiptr() == tail)
		tail = q.getiptr();
	if (q.getiptr() == head)
		head = p.getiptr();
	if (q.getiptr() == head)
		head = p.getiptr();
	//if swap involves some special node, i.e. head or tail then 
	//that data member of the list is updated

	T temp = p.getiptr()->getdata();
	p.getiptr()->setdata(q.getiptr()->getdata());
	q.getiptr()->setdata(temp);
}

template<class T>
void DLList<T>::SortList(Iterator p, Iterator q){
	//Insertion sort algorithm used
	for (Iterator j = p + 1; j != (q + 1); j++){
		T key;
		key = *j;
		Iterator k = j - 1;
		for (; k != p - 1 && key < *k; k--){
			(k + 1).getiptr()->setdata(*k);
		}
		if (k == NULL)
			head->setdata(key);
		else
			(k + 1).getiptr()->setdata(key);
	}
}

template<class T>
void DLList<T>::printlist(){
	//Prints the list with index of each list element
	//It facilitates the user to tell the exact element he wants the iterator
	//to point in order to perform operations
	node<T>* curr = head;
	for (int i = 0; curr != nullptr && i < size; i++, curr = curr->getnext()){
		cout << curr->getdata() << " ";
	}
	cout << endl;
	curr = nullptr;
}

template<class T>
DLList<T>::~DLList(){
	if (head)
		delete head;
	//only head deleted
	//rest is managed by our recursive node destructor
	//which deletes the next of every node followed by head
	else
		head = nullptr;
	tail = nullptr;
	//head and tail set to null after done with deletion 
	//to avoid dangling pointers
}

template<class T>
class DLList<T>::ListIterator{
	//Iterator class definition
private:
	node<T>* iptr;
	//pointer to a node in the list
public:
	ListIterator(node<T>* ptr){
		//parametric constructor
		iptr = ptr;
	}
	node<T>* getiptr(){
		//iptr getter
		return iptr;
	}
	T operator * (){
		//dereference
		return iptr->getdata();
	}
	ListIterator operator ++ (int){
		//postfix
		ListIterator temp = *this;
		if (iptr != NULL){
			iptr = iptr->getnext();
		}
		return temp;
	}
	ListIterator operator ++ (){
		//prefix
		if (iptr != NULL){
			iptr = iptr->getnext();
		}
		return *this;
	}
	ListIterator operator -- (int){
		//postfix
		ListIterator temp = *this;
		if (iptr != NULL){
			iptr = iptr->getprev();
		}
		return temp;
	}
	ListIterator operator -- (){
		//prefix
		if (iptr != NULL){
			iptr = iptr->getprev();
		}
		return *this;
	}
	bool operator == (const ListIterator &obj){
		//isequal
		return (iptr == obj.iptr);
	}
	bool operator != (const ListIterator &obj){
		//notequal
		return !(iptr == obj.iptr);
	}
	const ListIterator& operator = (const ListIterator& obj){
		//assignment operator
		iptr = obj.iptr;
		return *this;
	}
	ListIterator operator+ (int num){
		//ite=ite+int
		//It is needed while our printing with iterators
		//since we are not keeping any dummy node
		//so in order to print the last node as well
		//this way we can compare it with null without actually changing ite in our
		//printing loop to nullptr
		ListIterator temp(iptr);
		for (int i = 0; i < num; i++){
			temp++;
		}
		return temp;
	}

	ListIterator operator- (int num){
		//ite=ite-int
		ListIterator temp(iptr);
		for (int i = 0; i < num; i++){
			temp--;
		}
		return temp;
	}

	~ListIterator(){
		//destructor of iterator will not delete the ite
		//since it is a pointer to a list element which has to maintained
		//even if Iterator is deleted
		iptr = nullptr;
	}
};

template<class T>
class DLList<T>::ReverseListIterator{
private:
	node<T>* iptr;
public:
	ReverseListIterator(node<T>* ptr){
		iptr = ptr;
	}
	T operator * (){
		//dereference
		return iptr->getdata();
	}
	ReverseListIterator operator+ (int num){
		//ite=ite+int
		//ite=ite+int
		//It is needed while our printing with iterators
		//since we are not keeping any dummy node
		//so in order to print the last node as well
		//this way we can compare it with null without actually changing ite in our
		//printing loop to nullptr
		ReverseListIterator temp(iptr);
		for (int i = 0; i < num; i++){
			temp++;
		}
		return temp;
	}
	ReverseListIterator operator ++ (int){
		//postfix
		ReverseListIterator temp = *this;
		if (iptr != NULL){
			iptr = iptr->getprev();
		}
		return temp;
	}
	ReverseListIterator operator ++ (){
		//prefix
		if (iptr != NULL){
			iptr = iptr->getprev();
		}
		return *this;
	}
	ReverseListIterator operator -- (int){
		//postfix
		ReverseListIterator temp = *this;
		if (iptr != NULL){
			iptr = iptr->getnext();
		}
		return temp;
	}
	ReverseListIterator operator -- (){
		//prefix
		if (iptr != NULL){
			iptr = iptr->getnext();
		}
		return *this;
	}
	bool operator == (const ReverseListIterator &obj){
		//isequal
		return (iptr == obj.iptr);
	}
	bool operator != (const ReverseListIterator &obj){
		//notequal
		return !(iptr == obj.iptr);
	}
	const ReverseListIterator& operator = (const ReverseListIterator& obj){
		//assignment operator
		iptr = obj.iptr;
		return *this;
	}
	~ReverseListIterator(){
		//destructor of iterator will not delete the ite
		//since it is a pointer to a list element which has to maintained
		//even if Iterator is deleted
		iptr = nullptr;
	}
};