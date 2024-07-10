#ifndef _STARTSTRUCTTOKEN_H
#define _STARTSTRUCTTOKEN_H
#include "QScriptToken.h"

class StartStructToken : public QScriptToken {
    public:
        StartStructToken();
        ~StartStructToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_STARTSTRUCTTOKEN_H