#ifndef _MULTIPLYTOKEN_H
#define _MULTIPLYTOKEN_H
#include "QScriptToken.h"

class MultiplyToken : public QScriptToken {
    public:
        MultiplyToken();
        ~MultiplyToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_MULTIPLYTOKEN_H