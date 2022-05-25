
#include "hw3_symbol_table.hpp"
#include "hw3_enum_list.hpp"
#include "hw3_expression.hpp"
#include "hw3_master.hpp"
#include "hw3_output.hpp"

Scope::Scope(int new_offest, SCOPE_TYPE new_type) : symbol_list(), offest(new_offest), type(new_type) {}

//c'tor
SymbolTable::SymbolTable() : scope_list(), func_list(), seenMainFunc(false) , offsets() , instences(0)
{
    scope_list.emplace_back(0, GLOBAL_SCOPE);
    func_list.insert(FuncSymbol(Type(E_void),IDtype("print"),SymbolsList(vector<Symbol>(1, Symbol(IDtype(""), Type(E_string))))));
    func_list.insert(FuncSymbol(Type(E_void),IDtype("printi"),SymbolsList(vector<Symbol>(1, Symbol(IDtype(""), Type(E_int))))));
    offsets.push(0);
}

FuncList::iterator SymbolTable::findFunc(IDtype _id) 
{
    for (FuncList::iterator function = func_list.func_list.begin(); function != func_list.func_list.end(); function++)
    {
        if (function->id.id == _id.id)
        {
            return function;
        }
    }
    return func_list.func_list.end();
}

Symbol* SymbolTable::findSym(IDtype _id)
{
    for(vector<Scope>::iterator scope = scope_list.begin(); scope != scope_list.end(); scope++)
    {
        for(SymbolsList::iterator sym = (*scope).symbol_list.symbol_list.begin(); sym != (*scope).symbol_list.symbol_list.end() ; sym++)
        {
            if ((*sym).id.id == _id.id){
                return &(*sym);
            }
        }
    }

    for(SymbolsList::iterator sym = func_list.func_list.back().symbol_list.symbol_list.begin(); sym != func_list.func_list.back().symbol_list.symbol_list.end() ; sym++)
    {
        if ((*sym).id.id == _id.id)
        {
            return &(*sym);
        }
    }

    return NULL;
}

//SymbolTable



void SymbolTable::gg()
{
    checkMain();
    closeCurrentScope();
}

void SymbolTable::checkMain()
{
    if(!seenMainFunc){
        output::errorMainMissing();
        exit(1);
    }
}

void SymbolTable::openNewScope(SCOPE_TYPE type)
{
    output::printLog("Start openNewScope | size " + to_string(scope_list.size()));
    scope_list.emplace_back(offsets.top(), type);
    offsets.push(offsets.top());
    output::printLog("End openNewScope | size " + to_string(scope_list.size()));
}

void SymbolTable::openFuncScope(IDtype id, SymbolsList args, Type return_type)
{
    reverse(args.symbol_list.begin(),args.symbol_list.end());
    output::printLog("Flag " + id.id);
    if ((return_type == E_void) && (id.id == "main") && args.symbol_list.empty())
    {
        seenMainFunc = true;
    }
    if (findFunc(id) != func_list.func_list.end())
    {
        output::errorDef(yylineno, id.id);
        exit(1);
    }
    func_list.insert(FuncSymbol(return_type, id, args));
    for(SymbolsList::iterator sym = args.symbol_list.begin(); sym != args.symbol_list.end(); sym++)
    {
        if(findFunc((*sym).id) != func_list.func_list.end())
        {
            output::printLog("isId:" + (*sym).id.id);
            output::errorDef(yylineno, (*sym).id.id);
            exit(444);
        }
        for(SymbolsList::iterator sym2 = sym; sym2 != args.symbol_list.end(); sym2++)
        {
            if (sym == sym2)
                continue;
            if ((*sym).id.id == (*sym2).id.id)
            {
                output::printLog("isId2:" + (*sym).id.id);
                output::errorDef(yylineno, (*sym).id.id);
                exit(444);
            }
        }
    }
    offsets.push(0);
    scope_list.emplace_back(offsets.top(), FUNCTION_SCOPE);
}

void SymbolTable::triggerBreak()
{
    for(vector<Scope>::iterator i = scope_list.begin(); i != scope_list.end(); i++)
    {
        if ((*i).type == LOOP_SCOPE)
        {
            instences = 0;
            return;
        }
    }
    output::errorUnexpectedBreak(yylineno);
    exit(-1);
}

void SymbolTable::triggerContinue()
{
    for(vector<Scope>::iterator i = scope_list.begin(); i != scope_list.end(); i++){
        if ((*i).type == LOOP_SCOPE){
            return;
        }
    }
    output::errorUnexpectedContinue(yylineno);
    exit(-1);
}

Type SymbolTable::callFunc(IDtype funcName, ExpList arguments) 
{
    output::printLog("callFunc: start");
    if(findFunc(funcName) == func_list.func_list.end())
    {
        output::errorUndefFunc(yylineno, funcName.id);
        exit(1);
    }
    SymbolsList sArgs = SymbolsList();
    for (ExpList::iterator a = arguments.exp_list.begin(); a != arguments.exp_list.end(); a++)
    {
        sArgs.insert(Symbol(IDtype(""),(*a).type));
    }
    reverse(sArgs.symbol_list.begin(),sArgs.symbol_list.end());
    output::printLog("callFunc: Flag 1");
    FuncSymbol func = *findFunc(funcName);
    output::printLog("func size: " + to_string(func.symbol_list.symbol_list.size()));
    output::printLog("sArgs size: " + to_string(sArgs.symbol_list.size()));
    std::vector<string> strTypes = std::vector<string>();
    for (SymbolsList::iterator i = func.symbol_list.symbol_list.begin(); i != func.symbol_list.symbol_list.end(); i++)
    {
        strTypes.push_back((*i).type.getStr());
    }
    if(sArgs.symbol_list.size() != func.symbol_list.symbol_list.size())
    {
        output::errorPrototypeMismatch(yylineno, funcName.id, strTypes);
        exit(-1);
    }
    for (unsigned int i = 0; i < sArgs.symbol_list.size(); ++i) 
    {
        if( (sArgs.symbol_list[i].type.type != func.symbol_list.symbol_list[i].type.type) &&
                !(sArgs.symbol_list[i].type.type == E_byte && func.symbol_list.symbol_list[i].type.type == E_int))
        {
            output::printLog("Got: " + sArgs.symbol_list[i].type.getStr());
            output::printLog("Expected: " + func.symbol_list.symbol_list[i].type.getStr());
            output::errorPrototypeMismatch(yylineno, funcName.id, strTypes);
            exit(-1);
        }
    }
    return func.return_type;
}
void SymbolTable::closeCurrentScope()
{
    output::endScope();
    if (scope_list.empty())
    {
        output::printLog("ASSERT: closeCurrentScope - empty");
        return;
    }
    if (scope_list.size() == 1)
    {
        //Close global
        for (FuncList::iterator func = func_list.func_list.begin(); func != func_list.func_list.end(); ++func){
            std::vector<string> argTypes;
            string funcType = (*func).return_type.getStr();
            for(SymbolsList::iterator sym = (*func).symbol_list.symbol_list.begin(); sym != (*func).symbol_list.symbol_list.end(); ++sym){
                argTypes.push_back((*sym).type.getStr());
            }
            output::printID((*func).id.id, 0, output::makeFunctionType(funcType, argTypes));
        }
    } else {
        if (scope_list.size() == 2) 
        {
            //Func scope
            FuncSymbol func = func_list.func_list.back();
            for (unsigned int i = 0; i < func.symbol_list.symbol_list.size(); ++i) 
            {
                string typeForPrinting = func.symbol_list.symbol_list[i].type.getStr();
                output::printID(func.symbol_list.symbol_list[i].id.id, -1 - i, typeForPrinting);
            }
        }
        Scope closingScope = scope_list.back();
        offsets.pop();
        for (unsigned int i = 0; i < closingScope.symbol_list.symbol_list.size(); ++i) 
        {
            string typeForPrinting = closingScope.symbol_list.symbol_list[i].type.getStr();
            output::printID(closingScope.symbol_list.symbol_list[i].id.id, offsets.top() + i, typeForPrinting);
        }
    }
    scope_list.pop_back();
    output::printLog("closeCurrentScope - end flag");
}

void SymbolTable::checkReturnType(Expression exp)
{
    if(!exp.castType(func_list.func_list.back().return_type))
    {
        output::errorMismatch(yylineno);
        exit(1);
    }
}
void SymbolTable::checkReturnType()
{
    if(func_list.func_list.back().return_type != E_void)
    {
        output::errorMismatch(yylineno);
        exit(1);
    }
}

void SymbolTable::addSymbol(Type type, IDtype id)
{
    if(isId(id)) 
    {
        output::errorDef(yylineno, id.id);
        exit(-1);
    }
    scope_list.back().symbol_list.insert(Symbol(id, type));
    offsets.top()++;
}

Type SymbolTable::getTypeByID(IDtype _id)
{
    Symbol* sym = findSym(_id);
    if(!sym)
    {
        output::errorUndef(yylineno, _id.id);
        exit(-46);
    }
    return sym->type;
}

Expression SymbolTable::getExpByID(IDtype _id)
{
    output::printLog("getExp id:" + _id.id);
    Expression tmp = Expression(getTypeByID(_id));
    output::printLog("getExp res:" + tmp.type.getStr());
    return Expression(getTypeByID(_id));
}

void SymbolTable::assign(IDtype _id, Expression exp)
{
    Symbol* sym = findSym(_id);
    if(!sym)
    {
        output::errorUndef(yylineno, _id.id);
        exit(-463);
    }
    Expression newE = Expression(sym->type);
    newE = exp;
}