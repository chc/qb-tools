#ifndef _GREATEARTHANTOKEN_H
#define _GREATEARTHANTOKEN_H
#include "QScriptToken.h"

class GreaterThanToken : public QScriptToken {
    public:
        GreaterThanToken() {

        }
        ~GreaterThanToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_GREATERTHAN;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? "> " : ">";   
        }
    private:
};
#endif //_GREATEARTHANTOKEN_H