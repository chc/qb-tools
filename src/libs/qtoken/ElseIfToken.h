#ifndef _ELSEIFTOKEN_H
#define _ELSEIFTOKEN_H
#include "QScriptToken.h"

class ElseIfToken : public QScriptToken {
    public:
        ElseIfToken() {

        }
        ~ElseIfToken() {

        }
        EScriptToken GetType() {
            return ESCRIPTTOKEN_KEYWORD_ELSEIF;
        }
        void LoadParams(IStream *stream) {
            m_file_offset = stream->GetOffset()-sizeof(uint8_t);
            m_next_offset = stream->ReadUInt16();
            m_endif_offset = stream->ReadUInt16();
        }
        std::string ToString() {
            return AppendSpaceToString ? "elseif " : "elseif";
        }
        int GetPreTabOffset() {
            return -1;
        }
        int GetPostTabOffset() {
            return 1;
        }
        void Write(IStream *stream) {
            m_file_offset = stream->GetOffset();
            stream->WriteByte(ESCRIPTTOKEN_KEYWORD_ELSEIF);
            stream->WriteUInt16(m_next_offset);
            stream->WriteUInt16(m_endif_offset);
        }

        uint16_t GetNextOffset() { return m_next_offset; }
        uint16_t GetEndIfOffset() {return m_endif_offset; }
        void SetNextOffset(IStream *stream, uint16_t offset) {
            m_next_offset = offset;
            size_t cursor = stream->GetOffset();

            stream->SetCursor(m_file_offset+sizeof(uint8_t));
            stream->WriteUInt16(offset);

            stream->SetCursor(cursor);
        }
        void SetEndIfOffset(IStream *stream, uint16_t offset) {
            m_endif_offset = offset;

            size_t cursor = stream->GetOffset();

            stream->SetCursor(m_file_offset+sizeof(uint8_t)+sizeof(uint16_t));
            stream->WriteUInt16(offset);

            stream->SetCursor(cursor);
        }
        std::vector<TokenInjection> GetInjections()  {
            std::vector<TokenInjection> v;

            /*TokenInjection i;
            i.use_next_jump_offset = false;
            i.offset = m_next_offset - 2;
            i.token = "**ELSEIF1**";
            v.push_back(i);

            i.use_next_jump_offset = false;
            i.offset = m_endif_offset - 2;
            i.token = "**ELSEIF2**";
            v.push_back(i);*/
            return v;
        }

    private:
        uint16_t m_next_offset;
        uint16_t m_endif_offset;
};
#endif //_ELSEIFTOKEN_H