#ifndef _ARRAYSYMBOL_H
#define _ARRAYSYMBOL_H
#include "QSymbol.h"
#include <vector>

class ArraySymbol : public QSymbol {
    public:
        ArraySymbol();
        ArraySymbol(std::vector<QSymbol *> tokens);
        //
        ArraySymbol(QSymbol **tokens, uint32_t num_tokens);
        ~ArraySymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);        
        std::string ToString();
        uint32_t GetNumItems() { return m_num_items; };
        QSymbol *GetToken(uint32_t index) { return m_tokens[index]; }
        void Write(IStream *stream);
        void WriteToArray(IStream *stream);
    private:
        void WriteNoOffset(IStream *stream);
        uint32_t m_num_items;
        QSymbol **m_tokens;
};
#endif //_ARRAYSYMBOL_H