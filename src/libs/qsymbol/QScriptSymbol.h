#ifndef _QSCRIPTSYMBOL_H
#define _QSCRIPTSYMBOL_H
#include "QSymbolToken.h"

class QScriptSymbol : public QSymbolToken {
    public:
        QScriptSymbol();
        ~QScriptSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
        uint32_t GetDecompLen() { return m_decomp_len; }
        uint8_t *GetDecompBuff() {return m_decomp_buff; }
    private:
        uint8_t *m_decomp_buff;
        uint32_t m_decomp_len;
};
#endif //_QSCRIPTSYMBOL_H