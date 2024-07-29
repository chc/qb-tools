#ifndef _INTEGERSYMBOL_H
#define _INTEGERSYMBOL_H
#include "QSymbolToken.h"

class IntegerSymbol : public QSymbolToken {
    public:
        IntegerSymbol();
        ~IntegerSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
        int32_t GetValue() { return m_value; }
    private:
        int32_t m_value;
};
#endif //_INTEGERTOKEN_H