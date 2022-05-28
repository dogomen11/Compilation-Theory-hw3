#ifndef SYMTAB_H
#define SYMTAB_H
#include <string>
#include <vector>
#include "hw3_output.hpp"
#include "node.hpp"

using namespace std;
using namespace output;

class symbol {
public:
	string name;
	string type;
	int offset;
	symbol(string name, string type, int offset) : name(name), type(type), offset(offset) {}
	virtual ~symbol() = default;   
	virtual void printSymbol();
};

class function : public symbol
{
public:
	vector<string> args;
	function(string name, string type, vector<string>& args) :	symbol(name, type, 0), args(args){}
  void printSymbol() override;
};

class symbolTable{
private:
	
	vector<vector<symbol*>*> symtab;
	vector<int> offsets;


public:
	bool is_main = false;
	string func_return_type;
	symbolTable();
	void makeScope();
	void popScope();
	bool pushSymbol(string& name, string& type);
	bool pushFunction(string& name, string& type, vector<string>& args);
	symbol* checkIfValidId(myVariable* ID);
	function* checkIfValidIdFunc(myVariable* ID);
	symbol* existInTable(string& name);
	string pushArgs(vector<string>& args_name, vector<string>& args_type);

};



#endif