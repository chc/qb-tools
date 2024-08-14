#ifndef _FLOATTOKEN_H
#define _FLOATTOKEN_H
#include "QScriptToken.h"

class FloatToken : public QScriptToken {
    public:
        FloatToken();
        FloatToken(float v);
        ~FloatToken();
        EScriptToken GetType();
        void LoadParams(IStream *stream);
        void Write(IStream *stream);
        std::string ToString();
        float GetValue() { return m_value; }
    private:
        float m_value;
};
#endif //_FLOATTOKEN_H