#ifndef _IFTOKEN_H
#define _IFTOKEN_H
#include "QScriptToken.h"

class IfToken : public QScriptToken {
    public:
        IfToken();
        ~IfToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        int GetPostTabOffset();
    private:
};
#endif //_IFTOKEN_H