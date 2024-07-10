#ifndef _ENDSCRIPTTOKEN_H
#define _ENDSCRIPTTOKEN_H
#include "QScriptToken.h"

class EndScriptToken : public QScriptToken {
    public:
        EndScriptToken();
        ~EndScriptToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
        int GetPreTabOffset();
    private:
};
#endif //_ENDSCRIPTTOKEN_H