#ifndef _STRINGSYMBOL_H
#define _STRINGSYMBOL_H
#include "QSymbolToken.h"

class StringSymbol : public QSymbol {
    public:
        StringSymbol();
        StringSymbol(std::string value);
        ~StringSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string GetValue() { return m_value; }
        std::string ToString();
        void Write(IStream *stream);
        void WriteToArray(IStream *stream);
    private:
        void writeData(IStream *stream);
        std::string m_value;
};
#endif //_STRINGSYMBOL_H