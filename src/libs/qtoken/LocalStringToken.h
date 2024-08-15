#ifndef _LOCALSTRINGTOKEN_H
#define _LOCALSTRINGTOKEN_H
#include "QScriptToken.h"

class LocalStringToken : public QScriptToken {
    public:
        LocalStringToken();
        LocalStringToken(std::string value);
        ~LocalStringToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        std::string GetValue() { return m_value; }
    private:
        std::string m_value;
};
#endif //_LOCALSTRINGTOKEN_H