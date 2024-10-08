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
    m_file_offset = stream->GetOffset()-sizeof(uint8_t);
    m_checksum = stream->ReadUInt32();
}
void NameToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
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
        bool abnormal = false;
        int i = 0;
        while (name[i] != '\0') {
            //for (int i = 0; i < length; i++) {
            if ((name[i] >= 'a' && name[i] <= 'z') || (name[i] >= 'A' && name[i] <= 'Z') || name[i] == '_') {
            } else {
                if ((i > 0) && name[i] >= '0' && name[i] <= '9') {
                } else {
                    abnormal = true;
                    break;
                }
            }
            i++;
        }
        if (abnormal) {
            ss << "#\"" << name << "\"" << (AppendSpaceToString ? " " : "");
        } else {
            ss << name << (AppendSpaceToString ? " " : "");
        }
    } else {
        ss << "#\"0x" << std::hex << std::uppercase << m_checksum << "\"" << (AppendSpaceToString ? " " : "");
    }
    return ss.str();
}