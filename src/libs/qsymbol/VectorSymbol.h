#ifndef _VECTORSYMBOL_H
#define _VECTORSYMBOL_H
#include "QSymbolToken.h"

class VectorSymbol : public QSymbolToken {
    public:
        VectorSymbol();
        ~VectorSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
    private:
        float m_x;
        float m_y;
        float m_z;
};
#endif //_VECTORSYMBOL_H