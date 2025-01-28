#ifndef _OPENPARENTHESISTOKEN_H
#define _OPENPARENTHESISTOKEN_H
#include "QScriptToken.h"

class OpenParenthesisToken : public QScriptToken {
    public:
        OpenParenthesisToken() {

        }
        ~OpenParenthesisToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_OPENPARENTH;
        }
        void LoadParams(IStream *stream) {
            
        }
        std::string ToString() {
            return "(";
        }
    private:
};
#endif //_OPENPARENTHESISTOKEN_H