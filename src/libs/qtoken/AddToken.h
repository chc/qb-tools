#ifndef _ADDTOKEN_H
#define _ADDTOKEN_H
#include "QScriptToken.h"

class AddToken : public QScriptToken {
    public:
        AddToken();
        ~AddToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_ADDTOKEN_H