#ifndef HW3_ENUMS_LIST
#define HW3_ENUMS_LIST

#include <string>
extern int yylineno;


enum TYPE 
{
    E_default,
    E_void,
    E_int,
    E_byte,
    E_bool,
    E_auto,
    E_string
};


enum SCOPE_TYPE
{
    GLOBAL_SCOPE,
    REGULAR_SCOPE,
    IF_SCOPE,
    LOOP_SCOPE,
    FUNCTION_SCOPE
};

enum RELOPS
{
    E_L,
    E_G,
    E_LOE,
    E_GOE
};

const std::string types_string[] = 
{
    "VOID",
    "INT",
    "BYTE",
    "BOOL",
    "AUTO",
    "STRING"
};


#endif // HW3_ENUMS_LIST

