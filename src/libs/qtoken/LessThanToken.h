#ifndef _LESSTHANTOKEN_H
#define _LESSTHANTOKEN_H
#include "QScriptToken.h"

class LessThanToken : public QScriptToken {
    public:
        LessThanToken() {
            
        }
        ~LessThanToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_LESSTHAN;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "< " : "<";
        }
    private:
};
#endif //_LESSTHANTOKEN_H