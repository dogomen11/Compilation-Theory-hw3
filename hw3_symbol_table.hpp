 #ifndef HW3_SYMBOL_TABLE_HPP
#define HW3_SYMBOL_TABLE_HPP

#include "hw3_enum_list.hpp"
#include "hw3_expression.hpp"
#include "hw3_master.hpp"
#include "hw3_output.hpp"
#include <stack>
#include <string>
#include <algorithm>

using namespace std;


class Scope
{
    public:
        SymbolsList symbol_list;
        SCOPE_TYPE type;
        int offest;
        Scope(int new_offest, SCOPE_TYPE _type);
};


class SymbolTable
{
    public:
        vector<Scope> scope_list;
        FuncList func_list;
        bool seenMainFunc;
        stack<int> offsets;
        int instences;
        
//************************************************************************************************
    private:
        FuncList::iterator findFunc(IDtype _id);
        Symbol* findSym(IDtype _id);
        bool isId(IDtype _id) { return findSym(_id) || findFunc(_id) != func_list.func_list.end();  }
//************************************************************************************************
    public:

        SymbolTable(); //C'tor
        void gg();
        void checkMain();
        void openNewScope(SCOPE_TYPE type = REGULAR_SCOPE);
        void openLoopScope() { openNewScope(LOOP_SCOPE); }
        void openFuncScope(IDtype id, SymbolsList args, Type return_type);
        void triggerCase() { instences++ ; }
        void triggerBreak();
        void triggerContinue();
        Type callFunc(IDtype funcName, ExpList arguments);   
        void closeCurrentScope();
        void checkReturnType(Expression exp);
        void checkReturnType();
        void addSymbol(Type type, IDtype id);
        Type getTypeByID(IDtype _id);
        Expression getExpByID(IDtype _id);
        void assign(IDtype _id, Expression exp);
};

#endif //HW3_SYMBOL_TABLE_HPP
