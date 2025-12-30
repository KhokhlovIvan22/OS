#pragma once
#include <memory>
#include <ostream>
using std::shared_ptr;
using std::ostream;
using std::istream;

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
	shared_ptr<Node> getHead() const;
	void read(istream& in, int n);
	void print(ostream &out) const;
};
