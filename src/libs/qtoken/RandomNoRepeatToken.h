#ifndef _RANDOMNOREPEATTOKEN_H
#define _RANDOMNOREPEATTOKEN_H
#include "RandomToken.h"

class RandomNoRepeatToken : public RandomToken {
    public:
        RandomNoRepeatToken();
        ~RandomNoRepeatToken();
        EScriptToken GetType();
        std::string ToString();
    private:
};
#endif //_RANDOMNOREPEATTOKEN_H