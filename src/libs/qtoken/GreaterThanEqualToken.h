#ifndef _GREATERTHANEQUALTOKEN_H
#define _GREATERTHANEQUALTOKEN_H
#include "QScriptToken.h"

class GreaterThanEqualToken : public QScriptToken {
    public:
        GreaterThanEqualToken();
        ~GreaterThanEqualToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_GREATERTHANEQUALTOKEN_H