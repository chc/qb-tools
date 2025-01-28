#ifndef _COLONTOKEN_H
#define _COLONTOKEN_H
#include "QScriptToken.h"

class ColonToken : public QScriptToken {
    public:
        ColonToken() {

        }
        ~ColonToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_COLON;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return ":";
        }
    private:
};
#endif //_COLONTOKEN_H