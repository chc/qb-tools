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
        std::vector<QSymbolToken *> m_items;
};
#endif //_ARRAYSYMBOL_H