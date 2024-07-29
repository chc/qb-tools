#ifndef _ARRAYSYMBOL_H
#define _ARRAYSYMBOL_H
#include "QSymbolToken.h"
#include <vector>

class ArraySymbol : public QSymbolToken {
    public:
        ArraySymbol();
        ~ArraySymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);        
        std::string ToString();
    private:
        uint32_t m_num_items;
        QSymbolToken **m_tokens;
};
#endif //_ARRAYSYMBOL_H