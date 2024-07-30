#include "NameToken.h"
#include "IQStream.h"
#include "ChecksumNameToken.h"
#include <stdio.h>
#include <sstream>
NameToken::NameToken() {
    m_checksum_name = NULL;
    m_checksum = 0;
}
NameToken::NameToken(uint32_t v) {
    m_checksum_name = NULL;
    m_checksum = v;
}
NameToken::~NameToken() {

}
EScriptToken NameToken::GetType() {
    return ESCRIPTTOKEN_NAME;
}
void NameToken::LoadParams(IStream *stream) {
    m_checksum = stream->ReadUInt32();
}
void NameToken::Write(IStream *stream) {
    stream->WriteByte(ESCRIPTTOKEN_NAME);
    stream->WriteUInt32(m_checksum);
}
uint32_t NameToken::GetChecksum() {
    return m_checksum;
}
void NameToken::SetChecksumName(ChecksumNameToken *token) {
    m_checksum_name = token;
}
const char *NameToken::GetName() {
    if(m_checksum_name) {
        return m_checksum_name->GetName();
    }
    return nullptr;
}
std::string NameToken::ToString() {
    std::ostringstream ss;
    
    const char *name = GetName();
    if(name != nullptr) {
        ss << name << " ";
    } else {
        ss << "$" << m_checksum << "$ ";
    }
    return ss.str();
}