#ifndef _NAMESYMBOL_H
#define _NAMESYMBOL_H
#include "QSymbol.h"

class NameSymbol : public QSymbol {
    public:
        NameSymbol();
        NameSymbol(uint32_t v);
        ~NameSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
        uint32_t GetValue() { return m_value; }
        void Write(IStream *stream);
        void WriteToArray(IStream *stream);
    private:
        uint32_t m_value;
};
#endif //_NAMESYMBOL_H