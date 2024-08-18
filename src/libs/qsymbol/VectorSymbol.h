#ifndef _VECTORSYMBOL_H
#define _VECTORSYMBOL_H
#include "QSymbolToken.h"

class VectorSymbol : public QSymbolToken {
    public:
        VectorSymbol();
        VectorSymbol(float x, float y, float z);
        ~VectorSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
        float GetX() { return m_x; }
        float GetY() { return m_y; }
        float GetZ() { return m_z; }
        void Write(IStream *stream);
        void WriteToArray(IStream *stream);
    private:
        float m_x;
        float m_y;
        float m_z;
};
#endif //_VECTORSYMBOL_H