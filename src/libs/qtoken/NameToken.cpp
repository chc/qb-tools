#include "NameToken.h"
#include "IQStream.h"
#include "ChecksumNameToken.h"
#include <stdio.h>
#include <sstream>
NameToken::NameToken() {
    m_checksum_name = NULL;
}
NameToken::~NameToken() {

}
EScriptToken NameToken::GetType() {
    return ESCRIPTTOKEN_NAME;
}
void NameToken::LoadParams(IStream *stream) {
    m_checksum = stream->ReadUInt32();
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