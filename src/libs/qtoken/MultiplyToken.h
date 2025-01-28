#ifndef _MULTIPLYTOKEN_H
#define _MULTIPLYTOKEN_H
#include "QScriptToken.h"

class MultiplyToken : public QScriptToken {
    public:
        MultiplyToken() {

        }
        ~MultiplyToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_MULTIPLY;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "* " : "*";
        }
    private:
};
#endif //_MULTIPLYTOKEN_H