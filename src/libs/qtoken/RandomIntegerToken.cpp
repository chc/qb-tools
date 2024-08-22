#include "RandomIntegerToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

RandomIntegerToken::RandomIntegerToken() {

}
RandomIntegerToken::~RandomIntegerToken() {

}
EScriptToken RandomIntegerToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER;
}
void RandomIntegerToken::LoadParams(IStream *stream) {
}
void RandomIntegerToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_RANDOMINTEGER);
}
std::string RandomIntegerToken::ToString() {
    std::ostringstream ss;
    ss << "RandomInt()";
    return ss.str();
}