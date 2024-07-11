#ifndef _ARGTOKEN_H
#define _ARGTOKEN_H
#include "QScriptToken.h"

//This is an internal only token for injection purposes
class InjectedToken : public QScriptToken {
    public:
        InjectedToken(std::string token);
        ~InjectedToken();
        EScriptToken GetType();
        void LoadParams(IQStream *stream);
        std::string ToString();
    private:
        std::string m_token;
};
#endif //_ARGTOKEN_H