#ifndef _RANDOMPERMUTETOKEN_H
#define _RANDOMPERMUTETOKEN_H
#include "RandomToken.h"

class RandomPermuteToken : public RandomToken {
    public:
        RandomPermuteToken() : RandomToken(){

        }
        RandomPermuteToken(int num_items) : RandomToken(num_items){

        }
        ~RandomPermuteToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_RANDOM_PERMUTE;
        }
        std::string ToString() {
            return "RandomPermute(";
        }
    private:
};
#endif //_RANDOMPERMUTETOKEN_H