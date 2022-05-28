#ifndef HW3_ENUMS_HPP
#define HW3_ENUMS_HPP

#include <string>

extern int yylineno;

enum TYPE {
    E_void,
    E_int,
    E_byte,
    E_bool,
    E_string,
};

const std::string typeStr[] = {
        "VOID",
        "INT",
        "BYTE",
        "BOOL",
        "STRING",
};

enum RELOP_ENUM{
    E_L,
    E_G,
    E_LOE,
    E_GOE
};

enum SCOPE_REASON{
    GLOBAL_SCOPE,
    REGULAR_SCOPE,
    IF_SCOPE,
    LOOP_SCOPE,
    FUNC_SCOPE
};


#endif //HW3_ENUMS_HPP
