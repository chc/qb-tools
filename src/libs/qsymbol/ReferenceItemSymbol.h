#ifndef _REFERENCEITEMSYMBOL_H
#define _REFERENCEITEMSYMBOL_H
#include "QSymbolToken.h"

class ReferenceItemSymbol : public QSymbolToken {
    public:
        ReferenceItemSymbol(uint8_t type);
        ~ReferenceItemSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
        void SetValue(uint32_t v) { m_value = v;}
        uint32_t GetValue() { return m_value; }
        uint8_t GetRefType() { return m_type; }
        void Write(IStream *stream);
        void WriteToArray(IStream *stream);
    private:
        uint8_t m_type;
        uint32_t m_value;
};
#endif //_REFERENCEITEMSYMBOL_H