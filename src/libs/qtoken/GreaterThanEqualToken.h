#ifndef _GREATERTHANEQUALTOKEN_H
#define _GREATERTHANEQUALTOKEN_H
#include "QScriptToken.h"

class GreaterThanEqualToken : public QScriptToken {
    public:
        GreaterThanEqualToken() {

        }
        ~GreaterThanEqualToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_GREATERTHANEQUAL;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? ">= " : ">=";
        }
    private:
};
#endif //_GREATERTHANEQUALTOKEN_H