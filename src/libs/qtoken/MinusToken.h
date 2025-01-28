#ifndef _MINUSTOKEN_H
#define _MINUSTOKEN_H
#include "QScriptToken.h"

class MinusToken : public QScriptToken {
    public:
        MinusToken() {

        }
        ~MinusToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_MINUS;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "- " : "-";
        }
    private:
};
#endif //_MINUSTOKEN_H