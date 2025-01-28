#ifndef _STRINGQSTOKEN_H
#define _STRINGQSTOKEN_H
#include "QScriptToken.h"

#include <sstream>
#include <iomanip>


class StringQSToken : public QScriptToken {
    public:
        StringQSToken() {

        }
        StringQSToken(uint32_t checksum) : m_checksum(checksum) {

        }
        ~StringQSToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_STRINGQS;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
            m_checksum = stream->ReadUInt32();
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_STRINGQS);
            stream->WriteUInt32(m_checksum);
        }
        std::string ToString() {
            std::ostringstream ss;
            ss << "SQS(" << (int32_t)m_checksum << ") ";
            return ss.str();
        }
        uint32_t GetValue() { return m_checksum; }
    private:
        uint32_t m_checksum;
};
#endif //_STRINGQSTOKEN_H