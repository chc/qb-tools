#ifndef _STARTARRAYTOKEN_H
#define _STARTARRAYTOKEN_H
#include "QScriptToken.h"

class StartArrayToken : public QScriptToken {
    public:
        StartArrayToken();
        ~StartArrayToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
};
#endif //_STARTARRAYTOKEN_H