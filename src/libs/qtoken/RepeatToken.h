#ifndef _REPEATTOKEN_H
#define _REPEATTOKEN_H
#include "QScriptToken.h"

class RepeatToken : public QScriptToken {
    public:
        RepeatToken();
        ~RepeatToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPreTabOffset() { return -1; }
    private:
};
#endif //_REPEATTOKEN_H