#ifndef _NOTTOKEN_H
#define _NOTTOKEN_H
#include "QScriptToken.h"

class NotToken : public QScriptToken {
    public:
        NotToken() {

        }
        ~NotToken() {
            
        }
        EScriptToken GetType()  {
            return ESCRIPTTOKEN_KEYWORD_NOT;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "NOT " : "NOT";
        }
    private:
};
#endif //_NOTTOKEN_H