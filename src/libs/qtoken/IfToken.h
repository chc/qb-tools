#ifndef _IFTOKEN_H
#define _IFTOKEN_H
#include "QScriptToken.h"

class IfToken : public QScriptToken {
    public:
        IfToken() {

        }
        ~IfToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_IF;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return "if ";
        }
        int GetPostTabOffset() { return 1; }
    private:
};
#endif //_IFTOKEN_H