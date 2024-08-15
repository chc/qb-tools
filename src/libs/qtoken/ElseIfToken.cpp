#include "ElseIfToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

ElseIfToken::ElseIfToken() {

}
ElseIfToken::~ElseIfToken() {

}
EScriptToken ElseIfToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_ELSEIF;
}
void ElseIfToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset()-sizeof(uint8_t);
    m_next_offset = stream->ReadUInt16();
    m_endif_offset = stream->ReadUInt16();
}
void ElseIfToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_ELSEIF);
    stream->WriteUInt16(m_next_offset);
    stream->WriteUInt16(m_endif_offset);
}
void ElseIfToken::SetNextOffset(IStream *stream, uint16_t offset) {
    m_next_offset = offset;
    size_t cursor = stream->GetOffset();

    stream->SetCursor(m_file_offset+sizeof(uint8_t));
    stream->WriteUInt16(offset);

    stream->SetCursor(cursor);
}
void ElseIfToken::SetEndIfOffset(IStream *stream, uint16_t offset) {
    m_endif_offset = offset;

    size_t cursor = stream->GetOffset();

    stream->SetCursor(m_file_offset+sizeof(uint8_t)+sizeof(uint16_t));
    stream->WriteUInt16(offset);

    stream->SetCursor(cursor);
}
std::string ElseIfToken::ToString() {
    return AppendSpaceToString ? "elseif " : "elseif";
}
int ElseIfToken::GetPreTabOffset() {
    return -1;
}
int ElseIfToken::GetPostTabOffset() {
    return 1;
}
std::vector<TokenInjection> ElseIfToken::GetInjections() {
    std::vector<TokenInjection> v;

    TokenInjection i;
    i.use_next_jump_offset = false;
    i.offset = m_next_offset - 2;
    i.token = "**ELSEIF1**";
    v.push_back(i);

    i.use_next_jump_offset = false;
    i.offset = m_endif_offset - 2;
    i.token = "**ELSEIF2**";
    v.push_back(i);
    return v;
}