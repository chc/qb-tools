#ifndef _FLOATTOKEN_H
#define _FLOATTOKEN_H
#include "QScriptToken.h"

class FloatToken : public QScriptToken {
    public:
        FloatToken();
        ~FloatToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
        float m_value;
};
#endif //_FLOATTOKEN_H