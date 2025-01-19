#ifndef _RANDOMPERMUTETOKEN_H
#define _RANDOMPERMUTETOKEN_H
#include "RandomToken.h"

class RandomPermuteToken : public RandomToken {
    public:
        RandomPermuteToken();
        RandomPermuteToken(int num_items);
        ~RandomPermuteToken();
        EScriptToken GetType();
        std::string ToString();
    private:
};
#endif //_RANDOMPERMUTETOKEN_H