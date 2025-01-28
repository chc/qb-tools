#ifndef _ORTOKEN_H
#define _ORTOKEN_H
#include "QScriptToken.h"

class OrToken : public QScriptToken {
    public:
        OrToken() {

        }
        ~OrToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_OR;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "OR " : "OR";
        }
    private:
};
#endif //_ORTOKEN_H