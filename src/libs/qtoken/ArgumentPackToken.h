#ifndef _ARGUMENTPACKTOKEN_H
#define _ARGUMENTPACKTOKEN_H
#include "QScriptToken.h"


class ArgumentPackToken : public QScriptToken {
    public:
        ArgumentPackToken();
        ~ArgumentPackToken();
        virtual EScriptToken GetType();
        void LoadParams(IStream *stream);
        virtual std::string ToString();
    private:
};
#endif //_ARGUMENTPACKTOKEN_H