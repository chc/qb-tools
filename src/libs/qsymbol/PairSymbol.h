#ifndef _PAIRSYMBOL_H
#define _PAIRSYMBOL_H
#include "QSymbolToken.h"

class PairSymbol : public QSymbolToken {
    public:
        PairSymbol();
        PairSymbol(float x, float y);
        ~PairSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
        float GetX() { return m_x; }
        float GetY() { return m_y; }
        void Write(IStream *stream);
        void WriteToArray(IStream *stream);
    private:
        float m_x;
        float m_y;
};
#endif //_PAIRSYMBOL_H