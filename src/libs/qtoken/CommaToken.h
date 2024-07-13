#ifndef _COMMATOKEN_H
#define _COMMATOKEN_H
#include "QScriptToken.h"

class CommaToken : public QScriptToken {
    public:
        CommaToken();
        ~CommaToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_COMMATOKEN_H