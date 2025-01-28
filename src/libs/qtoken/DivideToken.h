#ifndef _DIVIDETOKEN_H
#define _DIVIDETOKEN_H
#include "QScriptToken.h"

class DivideToken : public QScriptToken {
    public:
        DivideToken() {

        }
        ~DivideToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_DIVIDE;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "/ " : "/";
        }
    private:
};
#endif //_DIVIDETOKEN_H