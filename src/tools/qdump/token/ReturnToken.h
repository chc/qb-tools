#ifndef _RETURNTOKEN_H
#define _RETURNTOKEN_H
#include "QScriptToken.h"

class ReturnToken : public QScriptToken {
    public:
        ReturnToken();
        ~ReturnToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_RETURNTOKEN_H