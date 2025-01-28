#ifndef _COMMATOKEN_H
#define _COMMATOKEN_H
#include "QScriptToken.h"

class CommaToken : public QScriptToken {
    public:
        CommaToken() {

        }
        ~CommaToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_COMMA;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? ", " : ",";
        }
    private:
};
#endif //_COMMATOKEN_H