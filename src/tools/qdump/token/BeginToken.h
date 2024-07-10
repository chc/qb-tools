#ifndef _BEGINTOKEN_H
#define _BEGINTOKEN_H
#include "QScriptToken.h"

class BeginToken : public QScriptToken {
    public:
        BeginToken();
        ~BeginToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_BEGINTOKEN_H