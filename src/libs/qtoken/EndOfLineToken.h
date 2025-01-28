#ifndef _ENDOFLINETOKEN_H
#define _ENDOFLINETOKEN_H
#include "QScriptToken.h"

class EndOfLineToken : public QScriptToken {
    public:
        EndOfLineToken() {

        }
        ~EndOfLineToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_ENDOFLINE;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_ENDOFLINE);
        }
        std::string ToString() {
            return "\n";
        }
    private:
};
#endif //_ENDOFLINETOKEN_H