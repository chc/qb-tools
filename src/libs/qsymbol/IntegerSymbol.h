#ifndef _INTEGERSYMBOL_H
#define _INTEGERSYMBOL_H
#include "QSymbol.h"

class IntegerSymbol : public QSymbol {
    public:
        IntegerSymbol();
        IntegerSymbol(int32_t v);
        ~IntegerSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
        int32_t GetValue() { return m_value; }
        void Write(IStream *stream);
        void WriteToArray(IStream *stream);
    private:
        int32_t m_value;
};
#endif //_INTEGERTOKEN_H