#ifndef NODE_H
#define NODE_H
#include <string>
//#include <vector>
using namespace std;

class Node
{
public:
	Node* next;
	explicit Node() : next(nullptr) {}
	virtual ~Node() = default;
};


class myType : public Node
{
public:
	string type;
	explicit myType(const string& type) : type(type) {}

};


class myExp : public Node
{
public:
	string type;
	explicit myExp(const string& type) : type(type) {}

};

class myVariable : public Node
{
public:
	string var;
	explicit myVariable(const string& yytext) : var(yytext) {}

};

class myNumber : public Node
{
public:
	int num_value;
	explicit myNumber(const string& yytext)
	{
		num_value = stoi(yytext);
	}
};

class myFormal : public Node
{
public:
	string name;
	string type;
	explicit myFormal(const string& name, const string& type) : name(name), type(type) {}
};




#endif