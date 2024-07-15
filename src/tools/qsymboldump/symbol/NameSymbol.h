#ifndef _NAMESYMBOL_H
#define _NAMESYMBOL_H
#include "QSymbolToken.h"

class NameSymbol : public QSymbolToken {
    public:
        NameSymbol();
        ~NameSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        std::string ToString();
    private:
        uint32_t m_value;
};
#endif //_NAMESYMBOL_H