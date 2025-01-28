#ifndef _CLOSEPARENTHESISTOKEN_H
#define _CLOSEPARENTHESISTOKEN_H
#include "QScriptToken.h"

class CloseParenthesisToken : public QScriptToken {
    public:
        CloseParenthesisToken() {

        }
        ~CloseParenthesisToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_CLOSEPARENTH;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? ") " : ")";
        }
    private:
};
#endif //_CLOSEPARENTHESISTOKEN_H