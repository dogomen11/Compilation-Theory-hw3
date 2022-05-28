#include "symtab.hpp"


symbolTable::symbolTable()
{
	vector<symbol*>* first = new vector<symbol*>;
	symtab.push_back(first);
	offsets.push_back(0);
	vector<string> args = { "STRING" };
	function* print_func = new function("print", "VOID", args);
	symtab.back()->push_back(print_func);
	args = { "INT" };
	function* printi_func = new function("printi", "VOID", args);
	symtab.back()->push_back(printi_func);
}

void symbolTable::makeScope()
{
	vector<symbol*>* t = new vector<symbol*>;
	symtab.push_back(t);
	offsets.push_back(offsets.back());
}

void function::printSymbol()
{
  string args_type = makeFunctionType(type, args);
	printID(name, offset, args_type);
}

void symbol::printSymbol()
{
	printID(name, offset, type);
}

void symbolTable::popScope()
{
	endScope();
	vector<symbol*>* top_scope = symtab.back();
  int scope_size = top_scope->size();
	for (int i = 0; i < scope_size; i++)
	{
		(*top_scope)[i]->printSymbol();
	}
	symtab.pop_back();
	offsets.pop_back();
}

bool symbolTable::pushSymbol(string& name, string& type)
{
	if (existInTable(name))
	{
		return false;
	}
	vector<symbol*>* top_scope = symtab.back();
	symbol* new_symbol = new symbol(name, type, offsets.back());
	offsets.back()++;
	top_scope->push_back(new_symbol);
	return true;
}

bool symbolTable::pushFunction(string& name, string& type, vector<string>& args)
{
	if (existInTable(name))
	{
		return false;
	}
	function* new_func = new function(name, type, args);
	if (name == "main" && type == "VOID" && args.empty())
	{
		is_main = true;
	}
	func_return_type = type;
	symtab.back()->push_back(new_func);
	return true;
}


symbol* symbolTable::existInTable(string& name)
{
  int table_size = symtab.size();
	for (int i = 0; i < table_size; i++)
	{
     int scope_size = symtab[i]->size();
		for (int j = 0; j < scope_size; j++)
		{
			if ((*symtab[i])[j]->name == name)
			{
				return (*symtab[i])[j];
			}
		}
	}
	return nullptr;
}

symbol* symbolTable::checkIfValidId(myVariable* ID)
{
	symbol* sym = existInTable(ID->var);
	function* func = dynamic_cast<function*>(sym);
	if (func || !sym)
	{
		return nullptr;
	}
	return sym;

}

function* symbolTable::checkIfValidIdFunc(myVariable* ID)
{
	symbol* sym = existInTable(ID->var);
	function* func = dynamic_cast<function*>(sym);
	if (!func)
	{
		return nullptr;
	}
	return func;
}

string symbolTable::pushArgs(vector<string>& args_name, vector<string>& args_type)
{ 
	int args_size = args_name.size();
	int j = -1;
	for (int i = 0; i < args_size; i++)
	{
		if (existInTable(args_name[i]) != nullptr)
		{
			return args_name[i];
		}
		symbol* new_symbol = new symbol(args_name[i], args_type[i], j);
		symtab.back()->push_back(new_symbol);
		j--;
	}
	return "";
}

