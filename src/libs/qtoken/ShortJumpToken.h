#ifndef _SHORTJUMPTOKEN_H
#define _SHORTJUMPTOKEN_H
#include "QScriptToken.h"

class ShortJumpToken : public QScriptToken {
    public:
        ShortJumpToken() {

        }
        ~ShortJumpToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_SHORTJUMP;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
            m_offset = stream->ReadUInt16();
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_SHORTJUMP);
            stream->WriteUInt16(m_offset);
        }
        std::string ToString() {
            return "";// "sjmp ";
        }
        uint16_t GetOffset() { return m_offset; };
        void RewriteOffset(IStream *stream, size_t diff) {
            size_t cursor = stream->GetOffset();

            stream->SetCursor(m_file_offset+1);
            stream->WriteUInt16(diff);

            stream->SetCursor(cursor);
        }
    private:
        uint32_t m_offset;
};
#endif //_SHORTJUMPTOKEN_H