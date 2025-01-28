#ifndef _ANDTOKEN_H
#define _ANDTOKEN_H
#include "QScriptToken.h"

class AndToken : public QScriptToken {
    public:
        AndToken() {

        }
        ~AndToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_AND;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "AND " : "AND";
        }
    private:
};
#endif //_ANDTOKEN_H