#ifndef _FASTELSETOKEN_H
#define _FASTELSETOKEN_H
#include "QScriptToken.h"

class FastElseToken : public QScriptToken {
    public:
        FastElseToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_FASTELSE;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset() - 1;
            m_offset = stream->ReadUInt16();
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_KEYWORD_FASTELSE);
            
            stream->WriteUInt16(0xFFFF);
        }
        std::string ToString() {
            return "else";
        }
        int GetPreTabOffset() {
            return -1;
        }
        int GetPostTabOffset() {
            return 1;
        }
        std::vector<TokenInjection> GetInjections() {
            std::vector<TokenInjection> v;
            /*TokenInjection i;
            i.use_next_jump_offset = false;
            i.offset = m_offset - 2;
            i.token = "**END ELSE2**";    
            v.push_back(i);*/
            return v;
        }
        uint16_t GetOffset() { return m_offset; };
        void RewriteOffset(IStream *stream, size_t diff) {
            size_t cursor = stream->GetOffset();

            stream->SetCursor(m_file_offset+1);
            stream->WriteUInt16(diff);

            stream->SetCursor(cursor);
        }
    private:
        uint16_t m_offset;
};
#endif //_FASTELSETOKEN_H