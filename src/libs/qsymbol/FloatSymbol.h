#ifndef _FLOATSYMBOL_H
#define _FLOATSYMBOL_H
#include "QSymbolToken.h"

class FloatSymbol : public QSymbolToken {
    public:
        FloatSymbol();
        ~FloatSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
    private:
        float m_value;
};
#endif //_FLOATSYMBOL_H