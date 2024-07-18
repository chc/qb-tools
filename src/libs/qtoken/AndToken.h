#ifndef _ANDTOKEN_H
#define _ANDTOKEN_H
#include "QScriptToken.h"

class AndToken : public QScriptToken {
    public:
        AndToken();
        ~AndToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_ANDTOKEN_H