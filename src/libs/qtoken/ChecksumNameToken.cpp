#include "./ChecksumNameToken.h"
#include "IQStream.h"

#include <stdio.h>
ChecksumNameToken::ChecksumNameToken() {
    m_name = NULL;
}
ChecksumNameToken::ChecksumNameToken(uint32_t checksum, const char *name) {
    m_checksum = checksum;
    m_name = strdup(name);

}
ChecksumNameToken::~ChecksumNameToken() {
    if(m_name) {
        free((void *)m_name);    
    }
}
EScriptToken ChecksumNameToken::GetType() {
    return ESCRIPTTOKEN_CHECKSUM_NAME;
}
void ChecksumNameToken::LoadParams(IStream *stream) {
    m_checksum = stream->ReadUInt32();
    m_name = stream->ReadNTS();
}
void ChecksumNameToken::Write(IStream *stream) {
    stream->WriteByte(ESCRIPTTOKEN_CHECKSUM_NAME);
    stream->WriteUInt32(m_checksum);
    stream->WriteNTS(m_name);
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