#include "QStream.h"
#include <QScriptToken.h>
#include <stdint.h>
#include <cassert>
#include <string.h>
QStream::QStream(IStream *data_stream) {
        mp_stream = data_stream;
}
QStream::~QStream() {
}
QScriptToken *QStream::NextToken() {
    uint8_t type = mp_stream->ReadByte();

    QScriptToken *result = nullptr;
    result = QScriptToken::Resolve(type);
    if(result) {
        result->LoadParams(mp_stream);
    }
    return result;
}
void QStream::WriteToken(IStream *stream, QScriptToken *token) {

}