#ifndef _INTEGERTOKEN_H
#define _INTEGERTOKEN_H
#include "QScriptToken.h"

class IntegerToken : public QScriptToken {
    public:
        IntegerToken();
        ~IntegerToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
        int32_t m_value;
};
#endif //_INTEGERTOKEN_H