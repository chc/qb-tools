#ifndef _ORTOKEN_H
#define _ORTOKEN_H
#include "QScriptToken.h"

class OrToken : public QScriptToken {
    public:
        OrToken();
        ~OrToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_ORTOKEN_H