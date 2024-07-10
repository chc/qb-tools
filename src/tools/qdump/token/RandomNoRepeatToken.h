#ifndef _RANDOMNOREPEATTOKEN_H
#define _RANDOMNOREPEATTOKEN_H
#include "QScriptToken.h"

class RandomNoRepeatToken : public QScriptToken {
    public:
        RandomNoRepeatToken();
        ~RandomNoRepeatToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_RANDOMNOREPEATTOKEN_H