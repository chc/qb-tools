#ifndef _BEGINTOKEN_H
#define _BEGINTOKEN_H
#include "QScriptToken.h"

class BeginToken : public QScriptToken {
    public:
        BeginToken();
        ~BeginToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_BEGINTOKEN_H