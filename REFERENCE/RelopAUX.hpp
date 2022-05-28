#ifndef HW3_RELOPAUX_HPP
#define HW3_RELOPAUX_HPP

#include "Enums.hpp"
#include "Exp_t.hpp"

class RelopAUX{
public:
    RELOP_ENUM op;

    RelopAUX() : op(E_L) {};
    RelopAUX(const string& _op){
        if(_op == "<"){
            op = E_L;
        }
        if(_op == ">"){
            op = E_G;
        }
        if(_op == "<="){
            op = E_LOE;
        }
        if(_op == ">="){
            op = E_GOE;
        }
    };
};

#endif //HW3_RELOPAUX_HPP
