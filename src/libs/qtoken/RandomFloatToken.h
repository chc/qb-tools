#ifndef _RANDOMFLOATTOKEN_H
#define _RANDOMFLOATTOKEN_H
#include "QScriptToken.h"

class RandomFloatToken : public QScriptToken {
    public:
        RandomFloatToken();
        ~RandomFloatToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
    private:
};
#endif //_RANDOMFLOATTOKEN_H