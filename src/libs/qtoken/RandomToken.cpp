#include "RandomToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

RandomToken::RandomToken() {
    m_offsets = nullptr;

}
RandomToken::~RandomToken() {
    if(m_offsets) {
        delete[] m_offsets;
    }
}
EScriptToken RandomToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_RANDOM;
}
void RandomToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset();
    m_num_items = stream->ReadUInt32();
    m_offsets = new RandomOffset[m_num_items];
    
    for(int i=0;i<m_num_items;i++) {
        uint16_t weight = stream->ReadUInt16();
        m_offsets[i].weight = weight;
    }
    
    std::ostringstream ss;
    for(int i=0;i<m_num_items;i++) {
        uint32_t o = stream->ReadUInt32();
        m_offsets[i].offset = o;
    }
    m_end_offset = stream->GetOffset();
    m_total_size = m_end_offset - m_file_offset;

}
void RandomToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(GetType());
    stream->WriteUInt32(m_num_items);
    for(int i=0;i<m_num_items;i++) {
        stream->WriteUInt16(m_offsets[i].weight);
    }

    for(int i=0;i<m_num_items;i++) {
        stream->WriteUInt32(m_offsets[i].offset);
    }
    m_end_offset = stream->GetOffset();
    m_total_size = m_end_offset - m_file_offset;
}
std::string RandomToken::ToString() {
    return "Random(";
}
std::vector<TokenInjection> RandomToken::GetInjections() {
    std::vector<TokenInjection> injections;

    for(int i=0;i<m_num_items;i++) {
        std::ostringstream ss;
        TokenInjection injection;
        injection.use_next_jump_offset = false;
        injection.offset = m_offsets[i].offset + CalculateTokenOffset(i) - m_total_size - 1; //-1 for skip token type byte

        ss << "@";
        if(m_offsets[i].weight != 1) {
            ss << "*" << m_offsets[i].weight;
        }
        injection.token = ss.str();
        injections.push_back(injection);
    }

    TokenInjection injection;
    injection.offset = 0;
    injection.use_next_jump_offset = true;
    injection.token = ")";
    injections.push_back(injection);
    return injections;
}