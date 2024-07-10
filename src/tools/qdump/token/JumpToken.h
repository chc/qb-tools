#ifndef _JUMPTOKEN_H
#define _JUMPTOKEN_H
#include "QScriptToken.h"

class JumpToken : public QScriptToken {
    public:
        JumpToken();
        ~JumpToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_JUMPTOKEN_H