#ifndef _RANDOMTOKEN_H
#define _RANDOMTOKEN_H
#include "QScriptToken.h"

class RandomToken : public QScriptToken {
    public:
        RandomToken();
        ~RandomToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_RANDOMTOKEN_H