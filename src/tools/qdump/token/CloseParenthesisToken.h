#ifndef _CLOSEPARENTHESISTOKEN_H
#define _CLOSEPARENTHESISTOKEN_H
#include "QScriptToken.h"

class CloseParenthesisToken : public QScriptToken {
    public:
        CloseParenthesisToken();
        ~CloseParenthesisToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_CLOSEPARENTHESISTOKEN_H