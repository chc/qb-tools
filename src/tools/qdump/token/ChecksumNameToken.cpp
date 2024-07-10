#include "./ChecksumNameToken.h"
#include "../IQStream.h"

#include <stdio.h>
ChecksumNameToken::ChecksumNameToken() {
    m_name = NULL;
}
ChecksumNameToken::~ChecksumNameToken() {
    if(m_name) {
        free((void *)m_name);    
    }
}
EScriptToken ChecksumNameToken::GetType() {
    return ESCRIPTTOKEN_CHECKSUM_NAME;
}
void ChecksumNameToken::LoadParams(IQStream *stream) {
    m_checksum = stream->ReadUInt32();
    m_name = stream->ReadNTS();
}
uint32_t ChecksumNameToken::GetChecksum() {
    return m_checksum;
}
const char *ChecksumNameToken::GetName() {
    return m_name;
}
std::string ChecksumNameToken::ToString() {
    return "";
}