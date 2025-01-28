#ifndef _ENDSTRUCTTOKEN_H
#define _ENDSTRUCTTOKEN_H
#include "QScriptToken.h"

class EndStructToken : public QScriptToken {
    public:
        EndStructToken() {

        }
        ~EndStructToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_ENDSTRUCT;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "} " : "}";
        }
    private:
};
#endif //_ENDSTRUCTTOKEN_H