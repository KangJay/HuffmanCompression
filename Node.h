#pragma once
#include <memory> 

template <class priority, class data>
class Node {
public:
	Node() {}
	Node(priority p, data& d) : priority_(p), data_(d), left_(nullptr), right_(nullptr), isLeafNode_(true) {}
	Node(priority p) : priority_(p), left_(nullptr), right_(nullptr) {}
	Node(bool isLeaf, priority p, data d) : data_(d), priority_(p), isLeafNode_(isLeaf) {}
	Node(bool isLeaf, data d) : data_(d), isLeafNode_(isLeaf) {}
	const data& getData() { return data_; }
	const priority& getPriority() { return priority_; }
	bool isLeaf() { return isLeafNode_; }
	void setLeft(Node<priority, data>* n) { left_ = n; }
	void setRight(Node<priority, data>* n) { right_ = n; }
	Node<priority, data>* getLeft() { return left_; }
	Node<priority, data>* getRight() { return right_; }
private:
	priority priority_;
	data data_;
	Node<priority, data>* left_;
	Node<priority, data>* right_;
	bool isLeafNode_;
	
};
