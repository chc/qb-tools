#ifndef _RANDOMINTEGERTOKEN_H
#define _RANDOMINTEGERTOKEN_H
#include "QScriptToken.h"

class RandomIntegerToken : public QScriptToken {
    public:
        RandomIntegerToken();
        ~RandomIntegerToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
    private:
};
#endif //_RANDOMINTEGERTOKEN_H