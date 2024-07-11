#ifndef _RANDOMTOKEN_H
#define _RANDOMTOKEN_H
#include "QScriptToken.h"


class RandomToken : public QScriptToken {
    public:
        RandomToken();
        ~RandomToken();
        virtual EScriptToken GetType();
        void LoadParams(IQStream *stream);
        virtual std::string ToString();
        std::vector<TokenInjection> GetInjections();
    private:
        std::vector<TokenInjection> m_injections;
};
#endif //_RANDOMTOKEN_H