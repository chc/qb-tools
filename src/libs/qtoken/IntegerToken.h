#ifndef _INTEGERTOKEN_H
#define _INTEGERTOKEN_H
#include "QScriptToken.h"

class IntegerToken : public QScriptToken {
    public:
        IntegerToken();
        IntegerToken(int32_t v);
        ~IntegerToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        int32_t GetValue() { return m_value; }
    private:
        int32_t m_value;
};
#endif //_INTEGERTOKEN_H