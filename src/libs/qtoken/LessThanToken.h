#ifndef _LESSTHANTOKEN_H
#define _LESSTHANTOKEN_H
#include "QScriptToken.h"

class LessThanToken : public QScriptToken {
    public:
        LessThanToken();
        ~LessThanToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_LESSTHANTOKEN_H