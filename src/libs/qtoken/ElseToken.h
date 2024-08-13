#ifndef _ELSETOKEN_H
#define _ELSETOKEN_H
#include "QScriptToken.h"

class ElseToken : public QScriptToken {
    public:
        ElseToken();
        ~ElseToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPreTabOffset() { return -1; }
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_ELSETOKEN_H