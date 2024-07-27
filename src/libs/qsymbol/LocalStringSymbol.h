#ifndef _LOCALSTRINGSYMBOL_H
#define _LOCALSTRINGSYMBOL_H
#include "QSymbolToken.h"

class LocalStringSymbol : public QSymbolToken {
    public:
        LocalStringSymbol();
        ~LocalStringSymbol();
        ESymbolType GetType();
        void LoadParams(IStream *stream);
        void LoadParamsFromArray(IStream *stream);
        std::string ToString();
    private:
        //int32_t m_value;
};
#endif //_LOCALSTRINGSYMBOL_H