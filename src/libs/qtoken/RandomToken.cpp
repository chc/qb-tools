#include "RandomToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

RandomToken::RandomToken() {

}
RandomToken::~RandomToken() {

}
EScriptToken RandomToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_RANDOM;
}
void RandomToken::LoadParams(IStream *stream) {
    size_t initial_offset = stream->GetOffset();
    uint32_t num_items = stream->ReadUInt32();
    RandomOffset *offsets = new RandomOffset[num_items];
    
    for(int i=0;i<num_items;i++) {
        uint16_t weight = stream->ReadUInt16();
        offsets[i].weight = weight;
    }
    
    std::ostringstream ss;
    for(int i=0;i<num_items;i++) {
        uint32_t o = stream->ReadUInt32();
        uint32_t diff = stream->GetOffset() - initial_offset;
        offsets[i].offset = o + diff;
    }
    int total_size = stream->GetOffset() - initial_offset;

    for(int i=0;i<num_items;i++) {
        std::ostringstream ss;
        TokenInjection injection;
        injection.use_next_jump_offset = false;
        injection.offset = offsets[i].offset - total_size;

        ss << "@";
        if(offsets[i].weight != 1) {
            ss << "*" << offsets[i].weight;
        }
        injection.token = ss.str();
        m_injections.push_back(injection);
    }

    TokenInjection injection;
    injection.offset = 0;
    injection.use_next_jump_offset = true;
    injection.token = ")";
    m_injections.push_back(injection);

    delete []offsets;
}
void RandomToken::Write(IStream *stream) {
    ///XXX: implement this
}
std::string RandomToken::ToString() {
    return "Random(";
}
std::vector<TokenInjection> RandomToken::GetInjections() {
    return m_injections;
}