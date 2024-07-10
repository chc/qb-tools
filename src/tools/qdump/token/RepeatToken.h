#ifndef _REPEATTOKEN_H
#define _REPEATTOKEN_H
#include "QScriptToken.h"

class RepeatToken : public QScriptToken {
    public:
        RepeatToken();
        ~RepeatToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_REPEATTOKEN_H