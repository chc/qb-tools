#ifndef _IQFILESTREAM_H
#define _IQFILESTREAM_H
#include <FileStream.h>
#include <stdio.h>
#include <IQStream.h>
#include <IStream.h>

class QStream : public IQStream {
    public:
        QStream(IStream *data_stream);
        ~QStream();
        QScriptToken *NextToken();
        void WriteToken(IStream *stream, QScriptToken *token);
        private:
            IStream *mp_stream;
};
#endif //_IQFILESTREAM_H