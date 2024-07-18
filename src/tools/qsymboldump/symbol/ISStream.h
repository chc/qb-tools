#ifndef _ISSTREAM_H
#define _ISSTREAM_H
#include <stdint.h>
#include <stddef.h>
class QSymbolToken;
class ISStream {
    public:
        virtual QSymbolToken *NextSymbol() = 0;
};
#endif //_ISSTREAM_H