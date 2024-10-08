#ifndef _QSCRIPTSYMBOL_H
#define _QSCRIPTSYMBOL_H
#include "QSymbol.h"

class QScriptSymbol : public QSymbol {
    public:
        QScriptSymbol();
        QScriptSymbol(uint8_t *decomp_buff, uint32_t decomp_len);
        ~QScriptSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        uint32_t GetDecompLen() { return m_decomp_len; }
        uint8_t *GetDecompBuff() {return m_decomp_buff; }
        void Write(IStream *stream);
        void WriteToArray(IStream *stream);
    private:
        uint8_t *m_decomp_buff;
        uint32_t m_decomp_len;
};
#endif //_QSCRIPTSYMBOL_H