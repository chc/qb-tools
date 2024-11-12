#include "ArgToken.h"
#include "IQStream.h"
#include "ChecksumNameToken.h"
#include <stdio.h>
#include <sstream>
//#include <iomanip>
#include <cassert>

ArgToken::ArgToken() {
}
ArgToken::ArgToken(uint32_t v) {
}
ArgToken::~ArgToken() {

}
EScriptToken ArgToken::GetType() {
    return ESCRIPTTOKEN_ARG;
}
void ArgToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
}
void ArgToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_ARG);
}
std::string ArgToken::ToString() {
    return "&";
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