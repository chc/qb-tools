#ifndef _FASTIFTOKEN_H
#define _FASTIFTOKEN_H
#include "QScriptToken.h"

class FastIfToken : public QScriptToken {
    public:
        FastIfToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_FASTIF;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - sizeof(uint8_t);
            m_offset = stream->ReadUInt16();
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_KEYWORD_FASTIF);
            
            stream->WriteUInt16(0xFFFF);
        }
        std::string ToString() {
            return AppendSpaceToString ? "if " : "if";
        }
        int GetPostTabOffset() {
            return 1;
        }
        std::vector<TokenInjection> GetInjections() {
            std::vector<TokenInjection> v;

            /*TokenInjection i;
            i.use_next_jump_offset = false;
            i.offset = m_offset - 2;
            i.token = "**END IF2**";
            v.push_back(i);*/
            return v;
        }
        uint16_t GetOffset() { return m_offset; };
        void RewriteOffset(IStream *stream, size_t diff) {
            size_t cursor = stream->GetOffset();

            stream->SetCursor(m_file_offset+sizeof(uint8_t));
            stream->WriteUInt16(diff);

            stream->SetCursor(cursor);
        }
    private:
        uint16_t m_offset;
};
#endif //_FASTIFTOKEN_H