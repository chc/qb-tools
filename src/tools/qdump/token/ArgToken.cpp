#include "ArgToken.h"
#include "../IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

ArgToken::ArgToken() {

}
ArgToken::~ArgToken() {

}
EScriptToken ArgToken::GetType() {
    return ESCRIPTTOKEN_ARG;
}
void ArgToken::LoadParams(IQStream *stream) {

}
std::string ArgToken::ToString() {
    return "<";
}
std::vector<TokenInjection> ArgToken::GetInjections() {
    std::vector<TokenInjection> result;
    TokenInjection injection;
    injection.offset = sizeof(uint32_t) + sizeof(uint8_t); //size of name token
    injection.token = ">";
    result.push_back(injection);
    return result;
}