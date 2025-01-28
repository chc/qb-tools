#ifndef _LESSTHANEQUALTOKEN_H
#define _LESSTHANEQUALTOKEN_H
#include "QScriptToken.h"

class LessThanEqualToken : public QScriptToken {
    public:
        LessThanEqualToken() {

        }
        ~LessThanEqualToken() {
            
        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_LESSTHANEQUAL;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "<= " : "<=";
        }
    private:
};
#endif //_LESSTHANEQUALTOKEN_H