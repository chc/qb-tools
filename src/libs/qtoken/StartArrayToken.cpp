#include "StartArrayToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

StartArrayToken::StartArrayToken() {

}
StartArrayToken::~StartArrayToken() {

}
EScriptToken StartArrayToken::GetType() {
    return ESCRIPTTOKEN_STARTARRAY;
}
void StartArrayToken::LoadParams(IStream *stream) {
    m_file_offset = stream->GetOffset() - sizeof(uint8_t);
}
std::string StartArrayToken::ToString() {
    return "[";
}