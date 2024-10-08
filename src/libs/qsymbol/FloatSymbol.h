#ifndef _FLOATSYMBOL_H
#define _FLOATSYMBOL_H
#include "QSymbol.h"

class FloatSymbol : public QSymbol {
    public:
        FloatSymbol();
        FloatSymbol(float v);
        ~FloatSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        float GetValue() { return m_value; }
        std::string ToString();

        void Write(IStream *stream);
        void WriteToArray(IStream *stream);
    private:
        float m_value;
};
#endif //_FLOATSYMBOL_H