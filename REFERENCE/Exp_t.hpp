#ifndef HW3_EXP_T_HPP
#define HW3_EXP_T_HPP

#include <iostream>
#include <string>
#include "hw3_output.hpp"
#include "Enums.hpp"
#include "BaseObj.hpp"

class Exp_t : public BaseObj{
public:
    Type t;
    explicit Exp_t(Type t) : t(t) {};
    Exp_t(Type t, int i) : t(t) 
    {
        if ( t == E_byte && i >= (1 << 8)){
            output::errorByteTooLarge(yylineno, i);
            exit(1);
        }
    };
    Exp_t(const Exp_t& c) = default;

    bool isBool() const {
        return t == E_bool;
    }
    bool isNumerical() const{
        return t == E_int || t == E_byte;
    }
	bool isString() const{
		return t == E_string;
	}
	
    Type getDualType(const Exp_t& e) const 
    {
        if (t == E_byte && e.t == E_byte)
            return Type(E_byte);

        return Type(E_int);
    }

    bool castType(Type newT)
    {
        if(newT == E_void || t == E_void){
            output::errorMismatch(yylineno);
            output::printLog("Casting void");
            exit(1);
        }
        if (newT == E_int && t == E_byte)
        {
            t = E_int;
            return true;
        }
        if (t == newT){
            return true;
        };
        return false;
    }

    Exp_t& operator=(const Exp_t& e){
        if (!Exp_t(e).castType(t)){
            output::errorMismatch(yylineno);
            output::printLog("Flag casting from " + e.t.getStr() + " to " + t.getStr());
            exit(-1);
        }
        return (*this);
    };
    
};


class ExpList : public BaseObj{
public:
    vector<Exp_t> expList;
    ExpList(const vector<Exp_t>& _expList) : expList(_expList) {}
    ExpList() : expList() {};

    void insert(Exp_t _e){
        expList.push_back(_e);
    }

    typedef vector<Exp_t>::iterator iterator;
};

#endif //HW3_EXP_T_HPP
