#ifndef _INJECTEDTOKEN_H
#define _INJECTEDTOKEN_H
#include "QScriptToken.h"

//This is an internal only token for injection purposes
class InjectedToken : public QScriptToken {
    public:
        InjectedToken(std::string token);
        ~InjectedToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
        std::string m_token;
};
#endif //_INJECTEDTOKEN_H