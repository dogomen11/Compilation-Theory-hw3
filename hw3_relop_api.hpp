#ifndef HW3_RELOP_API
#define HW3_RELOP_API

#include "hw3_enum_list.hpp"
#include "hw3_master.hpp"

class RelopAPI
{
    public:
        RELOPS opperator;
        
        RelopAPI() : opperator(E_L) {};

        RelopAPI(const string& new_opperator)
        {
            if(new_opperator == "<"){
                opperator = E_L;
            }
            if(new_opperator == ">"){
                opperator = E_G;
            }
            if(new_opperator == "<="){
                opperator = E_LOE;
            }
            if(new_opperator == ">="){
                opperator = E_GOE;
            }
        };
};

#endif //HW3_RELOP_API
