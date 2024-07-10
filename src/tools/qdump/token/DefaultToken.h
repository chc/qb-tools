#ifndef _DEFAULTTOKEN_H
#define _DEFAULTTOKEN_H
#include "QScriptToken.h"

class DefaultToken : public QScriptToken {
    public:
        DefaultToken();
        ~DefaultToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_DEFAULTTOKEN_H