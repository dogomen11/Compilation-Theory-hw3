#ifndef HW3_EXP_T_HPP
#define HW3_EXP_T_HPP

#include <iostream>
#include <string>
#include "hw3_output.hpp"
#include "hw3_enum_list.hpp"
#include "hw3_master.hpp"

class Expression : public Master
{    
    public:
        Type type;

        Expression(): type(E_default) {}   //default C'tor

        explicit Expression(Type new_exp_type) : type(new_exp_type) {}

        Expression(Type type, int i) : type(type) 
        {
            if ( (type == E_byte) && (i >= (1 << 8)) )
            {
                output::errorByteTooLarge(yylineno, std::to_string(i));
                exit(1); 
            }
        }

        Expression(const Expression& exp_to_copy) : type(exp_to_copy.type) {}
    
        bool isBool()       const { return type == E_bool;                      }

        bool isNumerical()  const {  return type == E_int || type == E_byte;    }

        bool isString()     const { return type == E_string;                       }

        Type getDualType(const Expression& exp) const 
        {
            if ( (type == E_byte) && (exp.type == E_byte) )
                return E_byte;
    
            return E_int;
        }

        bool castType(Type new_type)
        {
            if( (new_type == E_void) || (type == E_void) )
            {
                output::errorMismatch(yylineno);
                output::printLog("Casting void");
                exit(1);
            }
            if (new_type == E_int && type == E_byte)
            {
                type = E_int;
                return true;
            }
            if (type == new_type)
            {
                return true;
            }
            if (type == E_default)
            {
                type = new_type;
                return true;
            }
    
            return false;
        }
    
        Expression& operator=(const Expression& exp)
        {
            if (!Expression(exp).castType(type)){
                output::errorMismatch(yylineno);
                output::printLog("Flag casting from " + exp.type.getStr() + " to " + type.getStr());
                exit(-1);
            }
            return (*this);
        }
};


class ExpList : public Master
{
    public:
        vector<Expression> exp_list;

        ExpList(const vector<Expression>& exp_list_to_copy) : exp_list(exp_list_to_copy) {}
        ExpList() : exp_list() {};
        void insert(Expression new_exp) { exp_list.push_back(new_exp); }
        typedef vector<Expression>::iterator iterator; // maybe change
};

#endif //HW3_EXP_T_HPP
