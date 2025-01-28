#ifndef _INJECTEDTOKEN_H
#define _INJECTEDTOKEN_H
#include "QScriptToken.h"

//This is an internal only token for injection purposes
class InjectedToken : public QScriptToken {
    public:
        InjectedToken(std::string token) {
            m_token = token;
        }
        ~InjectedToken() {

        }
        EScriptToken GetType() {
            return (EScriptToken)-1;
        }
        void LoadParams(IStream *stream) {

        }
        std::string ToString() {
            return AppendSpaceToString ? (m_token + " ") : m_token;
        }
    private:
        std::string m_token;
};
#endif //_INJECTEDTOKEN_H