#ifndef _ELSETOKEN_H
#define _ELSETOKEN_H
#include "QScriptToken.h"

class ElseToken : public QScriptToken {
    public:
        ElseToken();
        ~ElseToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
        int GetPreTabOffset();
        int GetPostTabOffset();
    private:
};
#endif //_ELSETOKEN_H