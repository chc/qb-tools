#include "StartStructToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

StartStructToken::StartStructToken() {

}
StartStructToken::~StartStructToken() {

}
EScriptToken StartStructToken::GetType() {
    return ESCRIPTTOKEN_STARTSTRUCT;
}
void StartStructToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
}
std::string StartStructToken::ToString() {
    return "{";
}