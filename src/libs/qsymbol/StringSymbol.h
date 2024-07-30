#ifndef _STRINGSYMBOL_H
#define _STRINGSYMBOL_H
#include "QSymbolToken.h"

class StringSymbol : public QSymbolToken {
    public:
        StringSymbol();
        ~StringSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string GetValue() { return m_value; }
        std::string ToString();
    private:
        std::string m_value;
};
#endif //_STRINGSYMBOL_H