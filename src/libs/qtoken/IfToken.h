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
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_IFTOKEN_H