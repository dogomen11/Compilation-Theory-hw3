#ifndef HW3_ENUMS_HPP
#define HW3_ENUMS_HPP

#include <string>

extern int yylineno;

enum TYPE {
    E_def,
    E_bool,
    E_byte,
    E_int,
    E_string,
    E_void
};

const std::string typeStr[] = {
        "default",
        "BOOL",
        "BYTE",
        "INT",
        "STRING",
        "VOID"
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
    CASE_SCOPE,
    SWITCH_SCOPE,
    LOOP_SCOPE,
    FUNC_SCOPE
};


#endif //HW3_ENUMS_HPP
