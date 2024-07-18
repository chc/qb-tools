#ifndef _ENDOFLINETOKEN_H
#define _ENDOFLINETOKEN_H
#include "QScriptToken.h"

class EndOfLineToken : public QScriptToken {
    public:
        EndOfLineToken();
        ~EndOfLineToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_ENDOFLINETOKEN_H