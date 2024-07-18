#ifndef _MINUSTOKEN_H
#define _MINUSTOKEN_H
#include "QScriptToken.h"

class MinusToken : public QScriptToken {
    public:
        MinusToken();
        ~MinusToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_MINUSTOKEN_H