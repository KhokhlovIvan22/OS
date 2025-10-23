#pragma once
#include <memory>
using std::shared_ptr;

struct Node {
	int value;
	shared_ptr<Node> next;
	explicit Node(int value): value(value), next (nullptr){}
};

class List {
private:
	shared_ptr<Node> head;
	public:
	List(): head(nullptr) { }
	~List() = default;
	int size() const;
	void push_back(int value);
	shared_ptr<Node> head() const;
	List reverse() const;
};