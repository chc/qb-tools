#ifndef _RANDOMNOREPEATTOKEN_H
#define _RANDOMNOREPEATTOKEN_H
#include "RandomToken.h"

class RandomNoRepeatToken : public RandomToken {
    public:
        RandomNoRepeatToken() : RandomToken() {

        }
        RandomNoRepeatToken(int num_items) : RandomToken(num_items) {

        }
        ~RandomNoRepeatToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_RANDOM_NO_REPEAT;
        }
        std::string ToString() {
            return "RandomNoRepeat(";
        }
    private:
};
#endif //_RANDOMNOREPEATTOKEN_H