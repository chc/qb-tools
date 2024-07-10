#ifndef _ENDSTRUCTTOKEN_H
#define _ENDSTRUCTTOKEN_H
#include "QScriptToken.h"

class EndStructToken : public QScriptToken {
    public:
        EndStructToken();
        ~EndStructToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_ENDSTRUCTTOKEN_H