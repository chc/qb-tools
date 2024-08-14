#ifndef _IQSTREAM_H
#define _IQSTREAM_H
#include <stdint.h>
#include <stddef.h>
class QScriptToken;
class IStream;
class IQStream {
    public:
        virtual QScriptToken *NextToken() = 0;
        virtual void WriteToken(IStream *stream, QScriptToken *token) = 0;
};
#endif //_IQSTREAM_H