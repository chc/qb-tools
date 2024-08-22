#ifndef _REFERENCEITEMSYMBOL_H
#define _REFERENCEITEMSYMBOL_H
#include "QSymbol.h"

class ReferenceItemSymbol : public QSymbol {
    public:
        ReferenceItemSymbol(uint8_t type, bool is_required_param = false);
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
        bool GetIsRequiredParams() { return m_is_required_param; }
    private:
        uint8_t m_type;
        uint32_t m_value;
        bool m_is_required_param;
};
#endif //_REFERENCEITEMSYMBOL_H