#ifndef _BEGINTOKEN_H
#define _BEGINTOKEN_H
#include "QScriptToken.h"

class BeginToken : public QScriptToken {
    public:
        BeginToken() {

        }
        ~BeginToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_BEGIN;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return "begin";
        }
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_BEGINTOKEN_H