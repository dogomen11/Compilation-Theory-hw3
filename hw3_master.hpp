#ifndef HW3_MASTER
#define HW3_MASTER

#include "hw3_enum_list.hpp"
#include <string>
#include <utility>
#include <vector>

using namespace std;


class Master
{
    public:
        Master() = default;
};

#define YYSTYPE Master*


class Num: public Master
{
    public:
        int num;

        Num(int new_num) : num(new_num) {}
};


class IDtype : public Master
{
    public:
        string id;

        IDtype(string new_id) : id(new_id) {}
};

class Type: public Master 
{
    public:
        TYPE type;

        Type(TYPE new_type) : type(new_type) {}
        string getStr() const                         { return types_string[(int)(this->type)]; }
        bool operator==(const Type type_to_cmp) const { return type == type_to_cmp.type;        }
        bool operator==(const TYPE type_to_cmp) const { return type == type_to_cmp;             }
        bool operator!=(const Type type_to_cmp) const { return !(type_to_cmp == *this);         }
        bool operator!=(const TYPE type_to_cmp) const { return !(*this == type_to_cmp);         }
};

class Symbol : public Master 
{
    public:
        IDtype id;
        Type type;

        Symbol(const IDtype new_id, const Type new_type)  : id(new_id), type(new_type) {}
        Symbol(const Symbol& sym_to_copy) : id(sym_to_copy.id), type(sym_to_copy.type) {}
};

class SymbolsList : public Master 
{
    public:
        vector<Symbol> symbol_list;

        SymbolsList() : symbol_list() {}  //empty vector
        SymbolsList(const vector<Symbol> &symbol_list_to_copy) : symbol_list(symbol_list_to_copy) {}
        void insert(Symbol new_sym) {symbol_list.push_back(new_sym); }
        //maybe change
        typedef vector<Symbol>::iterator iterator;
};

class FuncSymbol : public Master 
{
    public:
        Type return_type;
        IDtype id;
        SymbolsList symbol_list;

        FuncSymbol(Type new_return_type, IDtype new_id, SymbolsList new_symbol_list) : return_type(new_return_type), id(new_id), symbol_list(new_symbol_list) {}
};

class FuncList : public Master 
{
    public:
        vector<FuncSymbol> func_list;

        FuncList() : func_list() {}
        FuncList(vector<FuncSymbol> & func_list_to_copy) : func_list(func_list_to_copy) {}
        void insert(FuncSymbol new_function) { func_list.push_back(new_function); }
        //maybe change
        typedef vector<FuncSymbol>::iterator iterator;
};



#endif //HW3_MASTER
