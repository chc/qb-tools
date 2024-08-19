#include "ArgToken.h"
#include "IQStream.h"
#include "ChecksumNameToken.h"
#include <stdio.h>
#include <sstream>
//#include <iomanip>
#include <cassert>

ArgToken::ArgToken() {
    m_checksum_name = NULL;
    m_checksum = 0;
}
ArgToken::ArgToken(uint32_t v) {
    m_checksum_name = NULL;
    m_checksum = v;
}
ArgToken::~ArgToken() {

}
EScriptToken ArgToken::GetType() {
    return ESCRIPTTOKEN_ARG;
}
void ArgToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
    uint8_t t = stream->ReadByte();
    assert(t == 0x16);
    m_checksum = stream->ReadUInt32();
}
void ArgToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_ARG);
    stream->WriteByte(ESCRIPTTOKEN_NAME);
    stream->WriteUInt32(m_checksum);
}
uint32_t ArgToken::GetChecksum() {
    return m_checksum;
}
void ArgToken::SetChecksumName(ChecksumNameToken *token) {
    m_checksum_name = token;
}
const char* ArgToken::GetName() {
    if (m_checksum_name) {
        return m_checksum_name->GetName();
    }
    return nullptr;
}
std::string ArgToken::ToString() {
    std::ostringstream ss;
    const char* name = GetName();
    if (name != nullptr) {
        bool abnormal = false;
        int i = 0;
        while (name[i] != '\0') {
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
//std::vector<TokenInjection> ArgToken::GetInjections() {
//    std::vector<TokenInjection> result;
//    TokenInjection injection;
//    injection.use_next_jump_offset = false;
//    injection.offset = sizeof(uint32_t) + sizeof(uint8_t); //size of name token
//    injection.token = "> ";
//    result.push_back(injection);
//    return result;
//}