#ifndef _IQSTREAM_H
#define _IQSTREAM_H
#include <stdint.h>
#include <stddef.h>
class QScriptToken;
class IQStream {
    public:
        virtual QScriptToken *NextToken() = 0;
};
#endif //_IQSTREAM_H