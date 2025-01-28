#ifndef _NOTEQUALTOKEN_H
#define _NOTEQUALTOKEN_H
#include "QScriptToken.h"

class NotEqualToken : public QScriptToken {
    public:
        NotEqualToken() {

        }
        ~NotEqualToken() {
            
        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_NOTEQUAL;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "!= " : "!=";
        }
    private:
};
#endif //_NOTTOKEN_H