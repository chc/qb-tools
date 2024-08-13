#ifndef _ENDSCRIPTTOKEN_H
#define _ENDSCRIPTTOKEN_H
#include "QScriptToken.h"

class EndScriptToken : public QScriptToken {
    public:
        EndScriptToken();
        ~EndScriptToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPreTabOffset() { return -1; }
    private:
};
#endif //_ENDSCRIPTTOKEN_H