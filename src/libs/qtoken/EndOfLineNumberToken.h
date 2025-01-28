#ifndef _ENDOFLINENUMBERTOKEN_H
#define _ENDOFLINENUMBERTOKEN_H
#include "QScriptToken.h"

class EndOfLineNumberToken : public QScriptToken {
    public:
        EndOfLineNumberToken() : EndOfLineNumberToken(0) {

        }
        EndOfLineNumberToken(uint32_t line_number) {
            m_line_number = line_number;
        }
        ~EndOfLineNumberToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_ENDOFLINENUMBER;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
            m_line_number = stream->ReadUInt32();
        }
        void Write(IStream* stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_ENDOFLINENUMBER);
            stream->WriteUInt32(m_line_number);
        }
        uint32_t GetLineNumber() {
            return m_line_number;
        }
        std::string ToString() {
            return "\n";
        }
    private:
        uint32_t m_line_number;
};
#endif //_ENDOFLINENUMBERTOKEN_H