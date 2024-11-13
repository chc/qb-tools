#include "RandomFloatToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>

RandomFloatToken::RandomFloatToken() {

}
RandomFloatToken::~RandomFloatToken() {

}
EScriptToken RandomFloatToken::GetType() {
    return ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT;
}
void RandomFloatToken::LoadParams(IStream *stream) {
}
void RandomFloatToken::Write(IStream *stream) {
    m_file_offset = stream->GetOffset();
    stream->WriteByte(ESCRIPTTOKEN_KEYWORD_RANDOMFLOAT);
}
std::string RandomFloatToken::ToString() {
    std::ostringstream ss;
    ss << "RandomFloat ";
    return ss.str();
}