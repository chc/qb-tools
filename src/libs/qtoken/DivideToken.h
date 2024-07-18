#ifndef _DIVIDETOKEN_H
#define _DIVIDETOKEN_H
#include "QScriptToken.h"

class DivideToken : public QScriptToken {
    public:
        DivideToken();
        ~DivideToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_DIVIDETOKEN_H