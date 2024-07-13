#ifndef _DOTTOKEN_H
#define _DOTTOKEN_H
#include "QScriptToken.h"

class DotToken : public QScriptToken {
    public:
        DotToken();
        ~DotToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_DOTTOKEN_H