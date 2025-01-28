#ifndef _DOTTOKEN_H
#define _DOTTOKEN_H
#include "QScriptToken.h"

class DotToken : public QScriptToken {
    public:
        DotToken() {

        }
        ~DotToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_DOT;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return ".";
        }
    private:
};
#endif //_DOTTOKEN_H