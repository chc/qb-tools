#ifndef _EQUALSTOKEN_H
#define _EQUALSTOKEN_H
#include "QScriptToken.h"

class EqualsToken : public QScriptToken {
    public:
        EqualsToken();
        ~EqualsToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
    private:
};
#endif //_EQUALSTOKEN_H