#ifndef _RANDOMRANGETOKEN_H
#define _RANDOMRANGETOKEN_H
#include "QScriptToken.h"

class RandomRangeToken : public QScriptToken {
    public:
        RandomRangeToken();
        ~RandomRangeToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_RANDOMRANGETOKEN_H