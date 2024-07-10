#ifndef _ENDARRAYTOKEN_H
#define _ENDARRAYTOKEN_H
#include "QScriptToken.h"

class EndArrayToken : public QScriptToken {
    public:
        EndArrayToken();
        ~EndArrayToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_ENDARRAYTOKEN_H