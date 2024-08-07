#ifndef _ENDIFTOKEN_H
#define _ENDIFTOKEN_H
#include "QScriptToken.h"

class EndIfToken : public QScriptToken {
    public:
        EndIfToken();
        ~EndIfToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        int GetPreTabOffset();
    private:
};
#endif //_ENDIFTOKEN_H