///Author: Ji Kang
///Homework Assignment 2: Huffman Compression
///CSS 343, Prof. Bernstein
///Priority Queue class
///Class implements the ADT, Priority Queue, or Minimum heap. 
///Utilizes a vector and an int array of size 256 to record all ASCII character occurrences. 
///Program will not count UNICODE characters, ONLY ASCII and extended ASCII. 
#pragma once
#include "Node.h"
#include <vector>
#include <string>
#include <iostream>
#include <memory>
template<class priority, class data>
class PrioHeap {
public:
	PrioHeap(std::string fileName);
	~PrioHeap();
	bool privHeapVerify();
	bool isEmpty();
	int getSize() { return size_; }
	Node<priority, data>* pop();
	void push(priority p, data d);
	void push(Node<priority, data>*, Node<priority, data>*, priority total);
	unsigned int getTotal() { return totalChars; }
	void test();
	int* getFreqTable() { return freqCount; }
	unsigned int freqCount[256] = { 0 };
private:
	void fixHeap(int index);
	void pushFix();
	int size_;
	std::string file;
	std::vector<Node<priority, data>*> minHeap;
	void swap(int ind1, int ind2);
	bool heapVerify(); //Debugging 
	unsigned int totalChars; 
	void freqTableDump(); //Debugging 
};
/*
Precondition: None
Postcondition: None
Constructor that'll initially take in all of the character occurrences and push them as nodes into our
vector. 
*/
template<class priority, class data>
inline PrioHeap<priority, data>::PrioHeap(std::string fileName)
{
	file = fileName;
	size_ = 0;
	totalChars = 0;
	std::ifstream inFile;
	inFile.open(file);
	char input;
	while (inFile.get(input)) {
		totalChars++;
		freqCount[(int)input]++;
	}
	inFile.close();

	Node<priority, data>* arbitraryNode = new Node<priority, data>();
	minHeap.push_back(arbitraryNode);
	unsigned char c;
	for (int i = 0; i < 256; i++) {
		c = (char)(i);
		if (freqCount[i] > 0) {
			push(freqCount[i], c);
		}
	}
	//freqTableDump();
}

/*
Precondition: None
Postcondition: Min heap will be empty
Cleans up any pointers that may be lingering around. 
*/
template<class priority, class data>
inline PrioHeap<priority, data>::~PrioHeap()
{
	minHeap.clear();
}

/*
Precondition: None
Postcondition: None
Private helper method to "heapify" the min heap after each push. 
*/
template<class priority, class data>
inline void PrioHeap<priority, data>::pushFix()
{
	int index = getSize();
	while (index != 1 && minHeap[index]->getPriority() < minHeap[index / 2]->getPriority()) {
		swap(index / 2, index);
		index /= 2;
	}
}

/*
Precondition: None
Postcondition: Returns a bool
Private helper method that'll check every node follows the heap property. 
*/
template<class priority, class data>
inline bool PrioHeap<priority, data>::privHeapVerify()
{
	for (int i = 1; i <= ((getSize() - 1) / 2); i++) {
		if (minHeap[i]->getPriority() > minHeap[i * 2]->getPriority() || minHeap[i]->getPriority() > minHeap[i * 2 + 1]->getPriority()) {
			return false;
		}
	}
	return true;
}

/*
Precondition: None
Postcondition: Returns a bool
Tells if our heap is empty or not. 
*/
template<class priority, class data>
inline bool PrioHeap<priority, data>::isEmpty()
{
	if (getSize() > 0) return true; 
	return false;
}


/*
Precondition: None
Postcondition: Returns a Node<priority, data> pointer. 
Removes the top element from our heap, fixes the heap, and returns that pointer. 
*/
template<class priority, class data>
Node<priority, data>* PrioHeap<priority, data>::pop()
{
	Node<priority, data>* min = minHeap[1];
	swap(1, getSize());
	minHeap.erase(minHeap.begin() + getSize());
	size_--; 
	fixHeap(1);
	return min;
}

/*
Precondition: None
Postcondition: None
Private helper method that helps me see heap top from bottom. 
*/
template<class priority, class data>
inline void PrioHeap<priority, data>::test()
{
	int total = 0; 
	Node<priority, data>* temp;
	while (getSize() != 0) {
		temp = pop();
		std::cerr << temp->getData() << " " << temp->getPriority() << std::endl; 
		total += temp->getPriority();
	}
	std::cerr << heapVerify() << std::endl; 
}

/*
Precondition: None
Postcondition: None
Private helper method that fixes the heap at a specific index and downward. 
1 to fix the entire heap, >1 to fix any other subtree of the heap. 
*/
template<class priority, class data>
inline void PrioHeap<priority, data>::fixHeap(int index)
{
	int smallerChild;
	int leftChild;
	int rightChild;

	smallerChild = 0;
	leftChild = index * 2;
	rightChild = index * 2 + 1;
	if (leftChild < getSize() && minHeap[leftChild]->getPriority() < minHeap[index]->getPriority()) {
		smallerChild = leftChild;
	}
	if (rightChild < getSize() && minHeap[rightChild]->getPriority() < minHeap[index]->getPriority()) {
		smallerChild = rightChild;
	}
	if (smallerChild != 0) {
		swap(smallerChild, index);
		fixHeap(index);
		fixHeap(smallerChild);
	}

	//Special case for this implementation when there are less than 3 nodes in the entire heap. (No right child). 
	//Since the function checks left AND right children, it will fail to do a swap in the case of not having a right child. 
	//Since right child will be 0 by default (the smallest). 
	if (getSize() == 2) {
		if (minHeap[1]->getPriority() > minHeap[2]->getPriority()) {
			swap(1, 2);
		}
	}
}

/*
Precondition: None
Postcondition: None
Push method that'll take priority and data, form a new node, and push that into our
heap. 
*/
template<class priority, class data>
inline void PrioHeap<priority, data>::push(priority p, data d)
{
	Node<priority, data>* node = new Node<priority, data>(p, d);
	minHeap.push_back(node); 
	size_++;
	pushFix();
}

/*
Precondition: None
Postcondition: None
Push method that will take pre-formed nodes, combine them as children, and push the
newly formed node in. Usually used for debugging and testing. 
*/
template<class priority, class data>
inline void PrioHeap<priority, data>::push(Node<priority, data>* n1, Node<priority, data>* n2, priority total)
{
	Node<priority, data>* combined = new Node<priority, data>(total); 
	combined->setLeft(n1);
	combined->setRight(n2);
	minHeap.push_back(combined);
	size_++;
	pushFix();
}

/*
Precondition: None
Postcondition: None
Since we implement this program with a vector, we can swap the indexes using the 
Left child = 2 * index 
and 
Right child = 2 * index + 1 Rule. 
*/
template<class priority, class data>
inline void PrioHeap<priority, data>::swap(int ind1, int ind2)
{
	if (ind1 > getSize() || ind2 > getSize()) {
		std::cerr << "Invalid indexes while swapping!" << std::endl;
	}
	else {
		Node<priority, data>* temp(minHeap[ind1]);
		minHeap[ind1] = minHeap[ind2];
		minHeap[ind2] = temp;
		temp = nullptr;
	}
}

/*
Precondition: None
Postcondition: Returns a bool
Public method. 
*/
template<class priority, class data>
inline bool PrioHeap<priority, data>::heapVerify()
{
	return privHeapVerify();
}

/*
Precondition: None
Postcondition: Prints to standard error 
Helper debugging method that shows relative frequencies using splat marks.
*/
template<class priority, class data>
inline void PrioHeap<priority, data>::freqTableDump()
{
	double numFreqLargest = minHeap[' ']->getPriority();
	int splatMarks = 0;
	std::cerr << "Relative Frequency Chart. Char's not shown do not have any frequency" << std::endl;
	for (int i = 1; i <= getSize(); i++) {
			splatMarks = static_cast<int>(ceil(30 * ((minHeap[i]->getPriority() / numFreqLargest)))); //Used 30 instead of 50 since 50 printed out just too many to comfortably see.  
			std::cerr << "Character: " << minHeap[i]->getData() << ": ";
			for (int i = 0; i < splatMarks; i++) {
				std::cerr << "*";
			}
			std::cerr << std::endl; 
	}
}


