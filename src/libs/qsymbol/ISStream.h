#ifndef _ISSTREAM_H
#define _ISSTREAM_H
#include <stdint.h>
#include <stddef.h>
class QSymbol;
class ISStream {
    public:
        virtual QSymbol *NextSymbol() = 0;
        virtual void WriteSymbol(QSymbol *symbol) = 0;
};
#endif //_ISSTREAM_H