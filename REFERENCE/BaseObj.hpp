#ifndef HW3_BASEOBJ_HPP
#define HW3_BASEOBJ_HPP

#include <string>
#include "Enums.hpp"
#include <vector>
#include <utility>

using namespace std;

class BaseObj{
public:
    explicit BaseObj() = default;
    virtual ~BaseObj() = default;
};

class IDtype : public BaseObj{
public:
    string id;
    int line_to_fix;
    explicit IDtype(string _id, int line_to_fix = 0) : id(_id) {};
};

class Type: public BaseObj 
{
public:
    TYPE t;
    explicit Type(TYPE new_t) : t(new_t) {};

    string getStr() const;

    bool operator==(const Type rhs) const;
    bool operator==(const TYPE rhs) const;
    bool operator!=(const Type rhs) const;
    bool operator!=(const TYPE rhs) const;
    void operator=(const TYPE new_type);
};

class Symbol : public BaseObj {
public:
    IDtype id;
    Type t;

    explicit Symbol(const IDtype id, const Type t);

    Symbol(const Symbol& _sym);
};

class SymList : public BaseObj {
public:
    vector<Symbol> symList;
    explicit SymList();
    SymList(const vector<Symbol> &symList);

    void insert(Symbol sym);

    typedef vector<Symbol>::iterator iterator;
};

class FuncSymbol : public BaseObj {
public:
    Type retType;
    IDtype id;
    SymList symList;

    explicit FuncSymbol(Type _retType, IDtype _id, SymList _symList);
};

class FuncList : public BaseObj {
public:
    vector<FuncSymbol> funcList;

    explicit FuncList();
    FuncList(vector<FuncSymbol> & _funcList);

    void insert(FuncSymbol func);

    typedef vector<FuncSymbol>::iterator iterator;
};

class Num: public BaseObj{
public:
    int val;
    explicit Num(int val);
};

#define YYSTYPE BaseObj*



#endif //HW3_BASEOBJ_HPP
