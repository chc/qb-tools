#include "ShortJumpToken.h"
#include "IQStream.h"

#include <stdio.h>
#include <sstream>
#include <iomanip>


ShortJumpToken::ShortJumpToken() {

}
ShortJumpToken::~ShortJumpToken() {

}
EScriptToken ShortJumpToken::GetType() {
    return ESCRIPTTOKEN_SHORTJUMP;
}
void ShortJumpToken::LoadParams(IStream *stream) {
    stream->ReadUInt16();

}
std::string ShortJumpToken::ToString() {
    return "sjmp ";
}