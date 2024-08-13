#ifndef _DEFAULTTOKEN_H
#define _DEFAULTTOKEN_H
#include "QScriptToken.h"

class DefaultToken : public QScriptToken {
    public:
        DefaultToken();
        ~DefaultToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPreTabOffset() { return -1; }
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_DEFAULTTOKEN_H