#ifndef _NOTTOKEN_H
#define _NOTTOKEN_H
#include "QScriptToken.h"

class NotToken : public QScriptToken {
    public:
        NotToken();
        ~NotToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_NOTTOKEN_H