#ifndef _ARGTOKEN_H
#define _ARGTOKEN_H
#include "QScriptToken.h"

class ArgToken : public QScriptToken {
    public:
        ArgToken();
        ~ArgToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        std::vector<TokenInjection> GetInjections();
    private:
};
#endif //_ARGTOKEN_H