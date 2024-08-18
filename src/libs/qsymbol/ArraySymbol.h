#ifndef _ARRAYSYMBOL_H
#define _ARRAYSYMBOL_H
#include "QSymbolToken.h"
#include <vector>

class ArraySymbol : public QSymbolToken {
    public:
        ArraySymbol();
        ArraySymbol(std::vector<QSymbolToken *> tokens);
        //
        ArraySymbol(QSymbolToken **tokens, uint32_t num_tokens);
        ~ArraySymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);        
        std::string ToString();
        uint32_t GetNumItems() { return m_num_items; };
        QSymbolToken *GetToken(uint32_t index) { return m_tokens[index]; }
        void Write(IStream *stream);
        void WriteToArray(IStream *stream);
    private:
        uint32_t m_num_items;
        QSymbolToken **m_tokens;
};
#endif //_ARRAYSYMBOL_H