#ifndef _NOTEQUALTOKEN_H
#define _NOTEQUALTOKEN_H
#include "QScriptToken.h"

class NotEqualToken : public QScriptToken {
    public:
        NotEqualToken();
        ~NotEqualToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_NOTTOKEN_H