#ifndef _ENDOFFILETOKEN_H
#define _ENDOFFILETOKEN_H
#include "QScriptToken.h"

class EndOfFileToken : public QScriptToken {
    public:
        EndOfFileToken();
        ~EndOfFileToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_ENDOFFILETOKEN_H