#ifndef _JUMPTOKEN_H
#define _JUMPTOKEN_H
#include "QScriptToken.h"

class JumpToken : public QScriptToken {
    public:
        JumpToken() {

        }
        ~JumpToken() {
            
        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_JUMP;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
            
            m_offset = stream->ReadUInt32();
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_JUMP);
            stream->WriteUInt32(m_offset);
        }
        std::string ToString() {
            //std::ostringstream ss;
            //ss << "JMP(" << m_offset << ")";
            //return ss.str();
            return "";
        }
        uint16_t GetOffset() { return m_offset; };
        void RewriteOffset(IStream *stream, size_t diff)  {
            size_t cursor = stream->GetOffset();

            stream->SetCursor(m_file_offset+sizeof(uint8_t));
            stream->WriteUInt32(diff);

            stream->SetCursor(cursor);
        }
    private:
        uint32_t m_offset;
};
#endif //_JUMPTOKEN_H