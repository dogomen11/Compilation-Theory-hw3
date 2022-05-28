#ifndef HW3_TABLE_HPP
#define HW3_TABLE_HPP

#include "Enums.hpp"
#include "Exp_t.hpp"
#include "BaseObj.hpp"
#include <string>
#include <vector>
#include "hw3_output.hpp"
#include <stack>
#include <algorithm>



using namespace std;

class Scope{
public:
    SymList symList;
    int offest;
    SCOPE_REASON type;

    Scope(int _offest, SCOPE_REASON _type) :
            symList(),
            offest(_offest),
            type(_type) {};
};

typedef vector<Scope> ScopeList;

class SymbolTable{
public:
    ScopeList scopeList;
    FuncList funcList;
    bool seenMainFunc;
    stack<int> offsets;
    int cases;

    SymbolTable() : scopeList(), funcList(), seenMainFunc(false) , offsets() , cases(0){
        scopeList.emplace_back(0, GLOBAL_SCOPE);
        funcList.insert(FuncSymbol(Type(E_void),IDtype("print"),SymList(vector<Symbol>(1, Symbol(IDtype(""), Type(E_string))))));
        funcList.insert(FuncSymbol(Type(E_void),IDtype("printi"),SymList(vector<Symbol>(1, Symbol(IDtype(""), Type(E_int))))));

        offsets.push(0);
    };
    void gg() {
        checkMain();
        closeCurrentScope();
    }
    void checkMain(){
        if(!seenMainFunc){
            output::errorMainMissing();
            exit(1);
        }
    }

    // open scopes
    void openNewScope(SCOPE_REASON type = REGULAR_SCOPE){
        output::printLog("Start openNewScope | size " + to_string(scopeList.size()));
        scopeList.emplace_back(offsets.top(), type);
        offsets.push(offsets.top());
        output::printLog("End openNewScope | size " + to_string(scopeList.size()));
    }
    void openLoopScope(){
        openNewScope(LOOP_SCOPE);
    }
    
    void openFuncScope(IDtype id, SymList args, Type retType) 
    {
        reverse(args.symList.begin(),args.symList.end());
        output::printLog("Flag " + id.id);
        if ((retType == E_void) && (id.id == "main") && args.symList.empty()){
            seenMainFunc = true;
        }

        if (findFunc(id) != funcList.funcList.end())
        {
            output::errorDef(yylineno, id.id);
            exit(1);
        }

        funcList.insert(FuncSymbol(retType, id, args));
        for(SymList::iterator sym = args.symList.begin(); sym != args.symList.end(); sym++){
            if(findFunc((*sym).id) != funcList.funcList.end()){
                output::printLog("isId:" + (*sym).id.id);
                output::errorDef(yylineno, (*sym).id.id);
                exit(444);
            }
            for(SymList::iterator sym2 = sym; sym2 != args.symList.end(); sym2++){
                if (sym == sym2)
                    continue;
                if ((*sym).id.id == (*sym2).id.id){
                    output::printLog("isId2:" + (*sym).id.id);
                    output::errorDef(yylineno, (*sym).id.id);
                    exit(444);
                }
            }
        }


        offsets.push(0);
        scopeList.emplace_back(offsets.top(), FUNC_SCOPE);
    }

    void triggerBreak(){
        for(ScopeList::iterator i = scopeList.begin(); i != scopeList.end(); i++){
            if ((*i).type == LOOP_SCOPE ){ 
                cases = 0;
                return;
            }
        }

        output::errorUnexpectedBreak(yylineno);
        exit(-1);
    }

    void triggerContinue(){
        for(ScopeList::iterator i = scopeList.begin(); i != scopeList.end(); i++){
            if ((*i).type == LOOP_SCOPE){
                return;
            }
        }
        output::errorUnexpectedContinue(yylineno);
        exit(-1);
    }

    Type callFunc(IDtype funcName, ExpList arguments) {
        output::printLog("callFunc: start");

        if(findFunc(funcName) == funcList.funcList.end()){
            output::errorUndefFunc(yylineno, funcName.id);
            exit(1);
        }

        SymList sArgs = SymList();
        for (ExpList::iterator a = arguments.expList.begin(); a != arguments.expList.end(); a++){
            sArgs.insert(Symbol(IDtype(""),(*a).t));
        }

        reverse(sArgs.symList.begin(),sArgs.symList.end());

        output::printLog("callFunc: Flag 1");

        FuncSymbol func = *findFunc(funcName);
        output::printLog("func size: " + to_string(func.symList.symList.size()));
        output::printLog("sArgs size: " + to_string(sArgs.symList.size()));

        std::vector<string> strTypes = std::vector<string>();
        for (SymList::iterator i = func.symList.symList.begin(); i != func.symList.symList.end(); i++){
            strTypes.push_back((*i).t.getStr());
        }

        if(sArgs.symList.size() != func.symList.symList.size()){
            output::errorPrototypeMismatch(yylineno, funcName.id, strTypes);
            exit(-1);
        }


        for (unsigned int i = 0; i < sArgs.symList.size(); ++i) {
            if(
                    (sArgs.symList[i].t.t != func.symList.symList[i].t.t) &&
                    !(sArgs.symList[i].t.t == E_byte && func.symList.symList[i].t.t == E_int)){
                output::printLog("Got: " + sArgs.symList[i].t.getStr());
                output::printLog("Expected: " + func.symList.symList[i].t.getStr());
                output::errorPrototypeMismatch(yylineno, funcName.id, strTypes);
                exit(-1);
            }
        }

        return func.retType;
    }
    void closeCurrentScope(){

        output::endScope();

        if (scopeList.empty()){
            output::printLog("ASSERT: closeCurrentScope - empty");
            return;
        }


        if (scopeList.size() == 1){
            //Close global
            for (FuncList::iterator func = funcList.funcList.begin(); func != funcList.funcList.end(); ++func){
                std::vector<string> argTypes;
                string funcType = (*func).retType.getStr();
                for(SymList::iterator sym = (*func).symList.symList.begin(); sym != (*func).symList.symList.end(); ++sym){
                    argTypes.push_back((*sym).t.getStr());
                }
                output::printID((*func).id.id, 0, output::makeFunctionType(funcType, argTypes));
            }
        } else {
            if (scopeList.size() == 2) {
                //Func scope
                FuncSymbol func = funcList.funcList.back();

                for (unsigned int i = 0; i < func.symList.symList.size(); ++i) {
                    string typeForPrinting = func.symList.symList[i].t.getStr();
                    output::printID(func.symList.symList[i].id.id, -1 - i,
                                    typeForPrinting);
                }
            }

            Scope closingScope = scopeList.back();
            offsets.pop();
            for (unsigned int i = 0; i < closingScope.symList.symList.size(); ++i) {
                string typeForPrinting = closingScope.symList.symList[i].t.getStr();
                output::printID(closingScope.symList.symList[i].id.id,
                                offsets.top() + i, typeForPrinting);
            }
        }


        scopeList.pop_back();
        output::printLog("closeCurrentScope - end flag");
    }

    void checkReturnType(Exp_t exp){
        if(!exp.castType(funcList.funcList.back().retType)){
            output::errorMismatch(yylineno);
            exit(1);
        }
    }


    void debugIsLife(Type t1, Type t2)
    {
        std::cout << " type 1 is   " << t1.getStr() << "  type 2 is   " << t2.getStr() << endl;
    }

    void debugIsLife2()
    {
        std::cout << "PASSED AUTO MISMATCH " << endl;
    }

    void checkAutoValid(Type t)
    {
        if (!( t == E_bool || t == E_int || t == E_byte))
		{
            std::cout << "DEBUGGGGG" << endl;
			output::errorMismatch(yylineno);
			exit(1);
		}
    }

    void checkReturnType(){
        if(funcList.funcList.back().retType != E_void){
            output::errorMismatch(yylineno);
            exit(1);
        }
    }

    void addSymbol(Type t, IDtype id)
    {
        if(isId(id)) 
        {
            output::errorDef(yylineno, id.id);
            exit(-1);
        }

        scopeList.back().symList.insert(Symbol(id, t));
        offsets.top()++;
    }

    Type getTypeByID(IDtype _id){
        Symbol* sym = findSym(_id);
        if(!sym){
            output::errorUndef(yylineno, _id.id);
            exit(-46);
        }
        return sym->t;
    }

    Exp_t getExpByID(IDtype _id){
        output::printLog("getExp id:" + _id.id);
        Exp_t tmp = Exp_t(getTypeByID(_id));
        output::printLog("getExp res:" + tmp.t.getStr());

        return Exp_t(getTypeByID(_id));
    }

    void assign(IDtype _id, Exp_t e)
    {
        Symbol* sym = findSym(_id);
        if(!sym){
            output::errorUndef(yylineno, _id.id);
            exit(-463);
        }
        Exp_t newE = Exp_t(sym->t);
        newE = e;
    }

private:
    FuncList::iterator findFunc(IDtype _id){
        for (FuncList::iterator f = funcList.funcList.begin(); f != funcList.funcList.end(); f++){
            if (f->id.id == _id.id){
                return f;
            }
        }
        return funcList.funcList.end();
    }
    Symbol* findSym(IDtype _id){
        for(ScopeList::iterator scope = scopeList.begin(); scope != scopeList.end(); scope++){
            for(SymList::iterator sym = (*scope).symList.symList.begin(); sym != (*scope).symList.symList.end() ; sym++){
                if ((*sym).id.id == _id.id){
                    return &(*sym);
                }
            }
        }

        for(SymList::iterator sym = funcList.funcList.back().symList.symList.begin(); sym != funcList.funcList.back().symList.symList.end() ; sym++){
            if ((*sym).id.id == _id.id){
                return &(*sym);
            }
        }

        return NULL;
    }
    bool isId(IDtype _id){
        return findSym(_id) || findFunc(_id) != funcList.funcList.end();
    }
};

#endif //HW3_TABLE_HPP
