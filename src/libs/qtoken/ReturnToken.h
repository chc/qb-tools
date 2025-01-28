#ifndef _RETURNTOKEN_H
#define _RETURNTOKEN_H
#include "QScriptToken.h"

class ReturnToken : public QScriptToken {
    public:
        ReturnToken() {

        }
        ~ReturnToken() {
            
        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_RETURN;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "return " : "return";
        }
    private:
};
#endif //_RETURNTOKEN_H