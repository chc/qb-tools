#ifndef _BREAKTOKEN_H
#define _BREAKTOKEN_H
#include "QScriptToken.h"

class BreakToken : public QScriptToken {
    public:
        BreakToken();
        ~BreakToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_BREAKTOKEN_H