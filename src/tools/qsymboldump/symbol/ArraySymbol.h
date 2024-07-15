#ifndef _ARRAYSYMBOL_H
#define _ARRAYSYMBOL_H
#include "QSymbolToken.h"

class ArraySymbol : public QSymbolToken {
    public:
        ArraySymbol();
        ~ArraySymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
};
#endif //_ARRAYSYMBOL_H